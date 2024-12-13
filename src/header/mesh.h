#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <stdlib.h>

typedef struct {
  vec3 Position;
  vec3 Normal;
  vec2 TexCoords;
} Vertex;

typedef struct {
  unsigned int id;
  char *type;
} Texture;

typedef struct {
  Vertex *vertices;
  unsigned int vertices_size;
  unsigned int vertices_count;
  unsigned int *indices;
  unsigned int indices_size;
  unsigned int indices_count;
  Texture *textures;
  unsigned int textures_size;
  unsigned int textures_count;
  unsigned int VAO, VBO, EBO;
} Mesh;

int initMesh(Mesh *self, Vertex *vertices, size_t vertices_size,
             unsigned int *indices, size_t indices_size, Texture *textures,
             size_t textures_size);
int drawMesh(Mesh *self, Shader *shader);
int setupMesh(Mesh *self);

#endif // MESH_H
