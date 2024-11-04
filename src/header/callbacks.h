#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <GLFW/glfw3.h>
void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

#endif // CALLBACKS_H
