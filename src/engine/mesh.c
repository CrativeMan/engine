#include <GL/glew.h>

#include "../header/logger.h"
#include "../header/mesh.h"
#include "GL/glext.h"

#define ID "Mesh"

void initializeMesh(Mesh *self, float *vertices, size_t verticesSize,
                    unsigned int *indices, size_t indicesSize,
                    long int *counter) {
  glGenVertexArrays(1, &self->VAO);
  glGenBuffers(1, &self->VBO);
  glGenBuffers(1, &self->EBO);
  glCheckError();

  self->verticesSize = verticesSize;
  self->verticesCount = self->verticesSize / sizeof(float);
  self->indicesSize = indicesSize;
  self->indicesCount = self->indicesSize / sizeof(unsigned int);

  glBindVertexArray(self->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
  glBufferData(GL_ARRAY_BUFFER, self->verticesSize, vertices, GL_STATIC_DRAW);
  glCheckError();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, self->indicesSize, indices,
               GL_STATIC_DRAW);
  glCheckError();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glCheckError();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glCheckError();

  self->id = *counter;
  (*counter)++;

  loggerInfo(ID, "Created Mesh (vC: %d, iC: %d) '%d'", self->verticesCount,
             self->indicesCount, self->id);
}

void deleteMesh(Mesh *mesh) {
  glDeleteVertexArrays(1, &mesh->VAO);
  glDeleteBuffers(1, &mesh->VBO);
  free(mesh->textures);

  loggerInfo(ID, "Deleted Mesh (vC: %d, iC: %d) '%d'", mesh->verticesCount,
             mesh->indicesCount, mesh->id);
}
