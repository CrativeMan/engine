#ifndef MESH_H
#define MESH_H

#include <stdlib.h>

typedef struct {
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  unsigned int *textures;
  unsigned int verticesCount;
  unsigned int indicesCount;
  unsigned int textureCount;
} Mesh;

Mesh initMesh(float *vertices, size_t verticesSize, unsigned int *indices,
              size_t indicesSize, char *textures[], size_t textureSize);
void deleteMesh(Mesh *mesh);

#endif // MESH_H
