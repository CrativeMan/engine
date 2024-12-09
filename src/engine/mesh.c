#include <GL/glew.h>

#include "../header/logger.h"
#include "../header/mesh.h"

#define ID "Mesh"

void initMeshes(Mesh self[], float *vertices, int size, int id[]) {
  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  self[0].VAO = cubeVAO;
  self[0].VBO = VBO;
  self[0].verticesSize = size;
  self[0].verticesCount = size / sizeof(float);
  self[0].id = id[0];

  self[1].VAO = lightCubeVAO;
  self[1].VBO = VBO;
  self[1].verticesSize = size;
  self[1].verticesCount = size / sizeof(float);
  self[1].id = id[1];
}

void deleteMesh(Mesh *mesh) {
  glDeleteVertexArrays(1, &mesh->VAO);
  loggerInfo(ID, "Deleted VAO '%d'", mesh->VAO);
  glDeleteBuffers(1, &mesh->VBO);
  loggerInfo(ID, "Deleted VBO '%d'", mesh->VBO);

  loggerInfo(ID, "Deleted Mesh (vC: %d) '%d'", mesh->verticesCount, mesh->id);
}
