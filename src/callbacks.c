#include "header/callbacks.h"
#include "header/camera.h"
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  (void)window;
  cameraProcessScrollInput(window, Camera * camera, (float)yoffset);
}
