#include "../header/callbacks.h"
#include "../header/camera.h"
#include "../header/image.h"
#include "GLFW/glfw3.h"

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

void scrollCallback(Camera *camera, float yoffset) {
  camera->fov -= yoffset;
  if (camera->fov < 1.0f)
    camera->fov = 1.0f;
  if (camera->fov > 45.0f)
    camera->fov = 45.0f;
}

void inputCallback(GLFWwindow *window, Camera *camera) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    saveFrameBufferToPng(window);

  cameraMove(window, camera);
}
