#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"

typedef struct {
  Mesh *meshes;
  unsigned int meshes_size;
  unsigned int meshes_count;
  char *directory;
} Model;

Model loadModel(char *path);
void drawModel(Model *model, Shader *shader);
processNode();
processMesh();
Texture *loadMaterialTextures();

#endif // MODEL_H
