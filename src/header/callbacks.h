#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <GLFW/glfw3.h>

#include "camera.h"

void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void mouseCallback(Camera *camera, double xpos, double ypos, bool *firstMouse);

#endif // CALLBACKS_H