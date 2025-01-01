#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <stdlib.h>

typedef struct {
  float x, y, z;
} Vec3;
#define VEC3(x) ((Vec3){(x), (x), (x)})

typedef struct {
  float x, y;
} Vec2;
#define VEC2(x) ((Vec2){(x), (x)})

typedef struct {
  Vec3 position;
  Vec3 normal;
  Vec2 texCoords;
} Vertex;

typedef struct {
  unsigned int id;
  char type[32];
  char path[256];
} Texture;

typedef struct {
  Vertex *vertices;
  unsigned int *indices;
  Texture *textures;
  unsigned int numVertices;
  unsigned int numIndices;
  unsigned int numTextures;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
} Mesh;

int setupMesh(Mesh *self);
int drawMesh(Mesh *self, Shader *shader);

#endif // MESH_H
