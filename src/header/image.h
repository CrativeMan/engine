#ifndef IMAGE_H
#define IMAGE_H

#include <GLFW/glfw3.h>

typedef struct {
  unsigned int id;
  int width;
  int height;
  int nrComponents;
} Texture;

Texture loadTexture(char const *path);
void saveFrameBufferToPng(GLFWwindow *window);

#endif // IMAGE_H
