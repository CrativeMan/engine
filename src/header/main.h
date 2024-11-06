#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "camera.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

typedef struct {
  GLFWwindow *windowId;
  char title[100];
} Window;

typedef struct {
  unsigned int shaderProgram;
  Window window;
  Camera camera;
} Global;

#endif // MAIN_H
