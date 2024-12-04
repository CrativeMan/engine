#include <GL/glew.h>

#include "../header/logger.h"
#include "../header/mesh.h"

#define ID "Mesh"

void tempMeshes(Mesh mesh[], float *vertices, int size, int id[]) {
  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  mesh[0].VAO = cubeVAO;
  mesh[0].VBO = VBO;
  mesh[0].verticesSize = size;
  mesh[0].verticesCount = size / sizeof(float);
  mesh[0].id = id[0];

  mesh[1].VAO = lightCubeVAO;
  mesh[1].VBO = VBO;
  mesh[1].verticesSize = size;
  mesh[1].verticesCount = size / sizeof(float);
  mesh[1].id = id[1];
}

void initializeMesh(Mesh *self, float *vertices, size_t verticesSize,
                    long int *counter) {
  glGenVertexArrays(1, &self->VAO);
  glGenBuffers(1, &self->VBO);
  glCheckError();

  self->verticesSize = verticesSize;
  self->verticesCount = self->verticesSize / sizeof(float);

  glBindBuffer(GL_ARRAY_BUFFER, self->VBO);
  glBufferData(GL_ARRAY_BUFFER, self->verticesSize, vertices, GL_STATIC_DRAW);

  glBindVertexArray(self->VAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  self->id = *counter;
  (*counter)++;

  loggerInfo(ID, "Created Mesh (vC: %d) '%d'", self->verticesCount, self->id);
}

void initializeMeshWithEBO(Mesh *self, float *vertices, size_t verticesSize,
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
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
