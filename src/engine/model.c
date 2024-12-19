#include "../header/model.h"

Model loadModel(char *path) {
  Model m;



  return m;
}

void drawModel(Model *model, Shader *shader) {
  for (unsigned int i = 0; i < model->meshes_count; i++) {
    drawMesh(&model->meshes[i], shader);
  }
}
