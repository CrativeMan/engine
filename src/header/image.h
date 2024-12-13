#ifndef IMAGE_H
#define IMAGE_H

#include <GLFW/glfw3.h>

Texture loadTexture(char const *path);
void saveFrameBufferToPng(GLFWwindow *window);

#endif // IMAGE_H
