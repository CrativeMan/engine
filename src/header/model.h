#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

typedef struct {
  Mesh *meshes;
  Texture *textures_loaded;
  unsigned int *textureIds;
  unsigned int numMeshes;
  unsigned int numTexturesLoaded;
  unsigned int numTextureIds;
  char directory[256];
} Model;

Model *loadModel(char *path);
void destroyModel(Model *model);
void drawModel(Model *model, Shader *shader);
Mesh processMesh(const struct aiMesh *mesh, const struct aiScene *scene,
                 Model *model);
void processNode(const struct aiNode *node, const struct aiScene *scene,
                 Model *model);
void loadMaterialTextures(struct aiMaterial *mat, enum aiTextureType type,
                          const char *typeName, Model *model, Mesh *mesh);

#endif // MODEL_H
