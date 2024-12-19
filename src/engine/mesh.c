#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

#include "../header/mesh.h"
#include "../header/utils.h"
#include "../header/logger.h"
#include "GL/gl.h"
#include "GL/glext.h"

#define ID "Mesh"

int initMesh(Mesh *self, Vertex *vertices, size_t vertices_size, unsigned int *indices, size_t indices_size, Texture *textures, size_t textures_size) {
  self->vertices = vertices;
  self->vertices_size = vertices_size;
  self->vertices_count = vertices_size / sizeof(Vertex);
  self->indices = indices;
  self->indices_size = indices_size;
  self->indices_count = indices_size / sizeof(unsigned int);
  self->textures = textures;
  self->textures_size = textures_size;
  self->textures_count = textures_size / sizeof(Texture);

  int res = setupMesh(self);
  ASSERT(res, "failed to setup mesh\n");

  return 0;
}

int setupMesh(Mesh *self) {
  glGenVertexArrays(1, &self->VAO);
  glGenBuffers(1, &self->VBO);
  glGenBuffers(1, &self->EBO);

  glBindVertexArray(self->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, self->VBO);

  glBufferData(GL_ARRAY_BUFFER, self->vertices_count * sizeof(Vertex), &self->vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, self->indices_count * sizeof(unsigned int), &self->indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);

  int err = glCheckError();
  return err;
}

int drawMesh(Mesh *self, Shader *shader) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < self->textures_count; i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    char numberStr[16];
    char uniformName[64];
    char *name = self->textures[i].type;
    if (strcmp(name, "texture_diffuse") == 0) {
      snprintf(numberStr, sizeof(numberStr), "%u", diffuseNr);
      diffuseNr++;
    }
    else if (strcmp(name, "texture_specular") == 0) {
      snprintf(numberStr, sizeof(numberStr), "%u", specularNr);
      specularNr++;
    }
    snprintf(uniformName, sizeof(uniformName), "material.%s%s", self->textures[i].type, numberStr);
    shaderSetFloat(shader->id, uniformName, (float)i);
    glBindTexture(GL_TEXTURE_2D, self->textures[i].id);
  }

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(self->VAO);
  glDrawElements(GL_TRIANGLES, self->indices_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  int err = glGetError();
  return err;
}
