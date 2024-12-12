#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "mesh.h"
#include "shader.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

typedef struct {
  GLFWwindow *id;
  char title[100];
} Window;

typedef struct {
  Shader shader[2];
  Window window;
  Camera camera;
  Mesh mesh[2];
  long int counter;
} Global;

#endif // MAIN_H
