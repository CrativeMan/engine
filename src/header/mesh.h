#ifndef MESH_H
#define MESH_H

#include "texture.h"
#include <stdlib.h>

typedef struct {
  int id;           // id of mesh
  unsigned int VAO; // VertexArrayObject
  unsigned int VBO; // VertexBufferObject
  unsigned int EBO; // ElementBuffer Object
  Texture texture;
  unsigned int verticesCount; // num of vertices
  unsigned int verticesSize;  // size of vertices
  unsigned int indicesCount;  // num of indices
  unsigned int indicesSize;   // size of indices
  int textureCount;
} Mesh;

#define MESH_NULL {0, 0, 0, 0, 0, 0};

void tempMeshes(Mesh mesh[], float *vertices, int size, int id[]);
void initializeMesh(Mesh *self, float *vertices, size_t verticesSize,
                    long int *counter);
void initializeMeshWithEBO(Mesh *self, float *vertices, size_t verticesSize,
                           unsigned int *indices, size_t indicesSize,
                           long int *counter);
void deleteMesh(Mesh *mesh);

#endif // MESH_H
