#ifndef MESH_H
#define MESH_H

#include "texture.h"
#include <stdlib.h>

typedef struct {
  int id;           // id of mesh
  unsigned int VAO; // VertexArrayObject
  unsigned int VBO; // VertexBufferObject
  unsigned int EBO; // ElementBuffer Object
  Texture texture[3];
  unsigned int verticesCount; // num of vertices
  unsigned int verticesSize;  // size of vertices
  unsigned int indicesCount;  // num of indices
  unsigned int indicesSize;   // size of indices
  int textureCount;
} Mesh;

#define MESH_NULL {0, 0, 0, 0, 0, 0};

void initMeshes(Mesh mesh[], float *vertices, int size, int id[]);
void deleteMesh(Mesh *mesh);

#endif // MESH_H
