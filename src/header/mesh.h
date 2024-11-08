#ifndef MESH_H
#define MESH_H

#include <stdlib.h>

typedef struct {
  unsigned int VAO;           // VertexArrayObject
  unsigned int VBO;           // VertexBufferObject
  unsigned int EBO;           // ElementBuffer Object
  unsigned int *textures;     // textures array TODO: turn into texture struct
  unsigned int verticesCount; // num of vertices
  unsigned int verticesSize;  // size of vertices
  unsigned int indicesCount;  // num of indices
  unsigned int indicesSize;   // size of indices
  int textureCount;
} Mesh;

#define MESH_NULL {0, 0, 0, 0, 0, 0};

void initializeMesh(Mesh *self, float *vertices, size_t verticesSize,
                    unsigned int *indices, size_t indicesSize);
Mesh initMesh(float *vertices, size_t verticesSize, unsigned int *indices,
              size_t indicesSize, char *textures[], size_t textureSize);
void deleteMesh(Mesh *mesh);

#endif // MESH_H
