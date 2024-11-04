#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

typedef struct {
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  unsigned int indicesCount;
} Mesh;

typedef struct {
  GLFWwindow *window;
  char title[100];
} Window;

#endif // MAIN_H
