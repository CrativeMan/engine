#ifndef IMAGE_H
#define IMAGE_H

#include <GLFW/glfw3.h>

typedef struct {
  unsigned int id;
  int width, height;
  int nrComponents;
} Image;

Image loadImage(char const *path, char directory[]);
void saveFrameBufferToPng(GLFWwindow *window);

#endif // IMAGE_H
