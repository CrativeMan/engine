#include "../header/model.h"
#include "../header/image.h"
#include "../header/logger.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

#include <stdlib.h>
#include <string.h>

Model *loadModel(char *path) {
  Model *model = (Model *)malloc(sizeof(Model));
  loggerInfo("Model", "Starting to load model from '%s'", path);
  if (!model) {
    loggerError("Model", "Failed to allocate model");
  }

  model->meshes = NULL;
  model->textures_loaded = NULL;
  model->textureIds = NULL;
  model->numMeshes = 0;
  model->numTexturesLoaded = 0;
  model->numTextureIds = 0;

  strncpy(model->directory, path, strrchr(path, '/') - path);
  model->directory[strrchr(path, '/') - path] = '\0';

  const struct aiScene *scene =
      aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    loggerError("Model", "Assimp error: %s", aiGetErrorString());
    free(model);
  }

  processNode(scene->mRootNode, scene, model);

  aiReleaseImport(scene);

  return model;
}

void processNode(const struct aiNode *node, const struct aiScene *scene,
                 Model *model) {
  loggerInfo("Model", "Starting to process node '%s'", node->mName);
  unsigned int i;
  for (i = 0; i < node->mNumMeshes; i++) {
    struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    model->numMeshes++;
    model->meshes = realloc(model->meshes, model->numMeshes * sizeof(Mesh));
    model->meshes[model->numMeshes - 1] = processMesh(mesh, scene, model);
  }

  for (i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, model);
  }
}

Mesh processMesh(const struct aiMesh *mesh, const struct aiScene *scene,
                 Model *model) {
  Mesh result = {0};

  result.vertices = malloc(mesh->mNumVertices * sizeof(Vertex));
  result.numVertices = mesh->mNumVertices;

  unsigned int i;
  // process vertices
  for (i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex = {0};

    vertex.Position.x = mesh->mVertices[i].x;
    vertex.Position.y = mesh->mVertices[i].y;
    vertex.Position.z = mesh->mVertices[i].z;

    if (mesh->mNormals) {
      vertex.Normal.x = mesh->mNormals[i].x;
      vertex.Normal.y = mesh->mNormals[i].y;
      vertex.Normal.z = mesh->mNormals[i].z;
    }

    if (mesh->mTextureCoords[0]) {
      vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
      vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
    }

    result.vertices[i] = vertex;
  }

  // process indices
  result.numIndices = 0;
  for (i = 0; i < mesh->mNumFaces; i++) {
    result.numIndices += mesh->mFaces[i].mNumIndices;
  }

  result.indices = malloc(result.numIndices * sizeof(unsigned int));
  unsigned int indexOffset = 0;

  for (i = 0; i < mesh->mNumFaces; i++) {
    struct aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      result.indices[indexOffset + j] = face.mIndices[j];
    }
    indexOffset += face.mNumIndices;
  }

  // process material
  if (mesh->mMaterialIndex != -1) {
    struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // load diffuse textures
    loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",
                         model, &result);

    // Load specular textures
    loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular",
                         model, &result);
  }

  setupMesh(&result);

  return result;
}

void loadMaterialTextures(struct aiMaterial *mat, enum aiTextureType type,
                          const char *typeName, Model *model, Mesh *mesh) {
  unsigned int textureCount = aiGetMaterialTextureCount(mat, type);

  for (unsigned int i = 0; i < textureCount; i++) {
    struct aiString str;
    aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL,
                         NULL);

    int skip = 0;
    for (unsigned j = 0; j < model->numTexturesLoaded; j++) {
      if (strcmp(model->textures_loaded[j].path, str.data)) {
        mesh->textures =
            realloc(mesh->textures, (mesh->numTextures + 1) * sizeof(Texture));
        mesh->textures[mesh->numTextures] = model->textures_loaded[j];
        mesh->numTextures++;
        skip = 1;
        break;
      }
    }

    if (!skip) {
      Texture texture = {0};
      Image img = loadImage(str.data, model->directory);
      texture.id = img.id;

      strncpy(texture.type, typeName, sizeof(texture.type) - 1);
      strncpy(texture.path, str.data, sizeof(texture.path) - 1);

      size_t path_len = strlen(str.data);
      if (path_len >= sizeof(texture.path)) {
        memcpy(texture.path, str.data, sizeof(texture.path) - 1);
        texture.path[sizeof(texture.path) - 1] = '\0';
      } else {
        memcpy(texture.path, str.data, path_len);
        texture.path[path_len] = '\0';
      }

      mesh->textures =
          realloc(mesh->textures, (mesh->numTextures + 1) * sizeof(Texture));
      mesh->textures[mesh->numTextures] = texture;
      mesh->numTextures++;

      model->textures_loaded =
          realloc(model->textures_loaded,
                  (model->numTextureIds + 1) * sizeof(unsigned int));
      model->textures_loaded[model->numTexturesLoaded] = texture;
      model->numTexturesLoaded++;
    }
  }
}

void destroyModel(Model *model) {
  if (!model)
    return;

  for (unsigned int i = 0; i < model->numMeshes; i++) {
    free(model->meshes[i].vertices);
    free(model->meshes[i].indices);
    free(model->meshes[i].textures);
  }
  free(model->meshes);

  free(model->textures_loaded);
  free(model->textureIds);

  free(model);
}

void drawModel(Model *model, Shader *shader) {
  for (unsigned int i = 0; i < model->numMeshes; i++) {
    drawMesh(&model->meshes[i], shader);
  }
}
