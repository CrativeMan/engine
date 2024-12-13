#include <GL/glew.h>
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

  int res = glCheckError();
  return res;
}

int drawMesh(Mesh *self, Shader *shader) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < self->textures_count; i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    int number;
    char *name = self->textures[i].type;
    if (strcmp(name, "texture_diffuse") == 0)
      number = diffuseNr++;
    else if (strcmp(name, "texture_specular") == 0)
      number = specularNr++;

    shaderSetFloat(shader->id, strcat(strcat("material.", name), ), i);

  }
}
