#include "header/callbacks.h"
#include "header/camera.h"

void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void mouseCallback(Camera *camera, double xpos, double ypos, bool *firstMouse) {
  if (*firstMouse) {
    camera->lastX = xpos;
    camera->lastY = ypos;
    *firstMouse = false;
  }

  float xoffset = xpos - camera->lastX;
  float yoffset = camera->lastY - ypos;

  camera->lastX = xpos;
  camera->lastY = ypos;
  xoffset *= camera->sensitivity;
  yoffset *= camera->sensitivity;

  camera->yaw += xoffset;
  camera->pitch += yoffset;

  if (camera->pitch > 89.0f)
    camera->pitch = 89.0f;
  if (camera->pitch < -89.0f)
    camera->pitch = -89.0f;
}
