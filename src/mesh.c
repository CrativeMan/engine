#include "header/mesh.h"
#include <GL/glew.h>
#include <stdlib.h>

Mesh initMesh(float *vertices, size_t verticesSize, float *indices,
              size_t indicesSize, unsigned int *texture, size_t textureSize) {
  Mesh mesh;

  glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);
  glGenBuffers(1, &mesh.EBO);

  glBindVertexArray(mesh.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

  // vertex attrib pointers
  // pointer to the position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // pointer to the color values
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // pointer to the tex coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  mesh.verticesCount = verticesSize / (5 * sizeof(float));
  mesh.indicesCount = indicesSize / sizeof(unsigned int);
  mesh.textureCount = textureSize / sizeof(unsigned int);

  mesh.textures = (unsigned int *)malloc(textureSize);
  for (size_t i = 0; i < mesh.textureCount; i++) {
    mesh.textures[i] = texture[i];
  }

  return mesh;
}
