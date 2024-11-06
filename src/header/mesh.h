#ifndef MESH_H
#define MESH_H

typedef struct {
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  unsigned int *textures;
  unsigned int verticesCount;
  unsigned int indicesCount;
  unsigned int textureCount;
} Mesh;

#endif // MESH_H
