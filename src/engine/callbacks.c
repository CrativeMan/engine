#include "../header/callbacks.h"
#include "../header/camera.h"
#include "GLFW/glfw3.h"
#include "cglm/types.h"

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

void inputCallback(GLFWwindow *window, Camera *camera, bool *debug) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    *debug = true;

  // camera controls
  camera->cameraSpeed = 2.5f * camera->deltaTime;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    vec3 scaledUp;
    glm_vec3_scale(camera->cameraUp, camera->cameraSpeed, scaledUp);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      glm_vec3_add(camera->cameraPos, scaledUp, camera->cameraPos);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      glm_vec3_sub(camera->cameraPos, scaledUp, camera->cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vec3 scaledFront;
    glm_vec3_scale(camera->cameraFront, camera->cameraSpeed, scaledFront);
    glm_vec3_add(camera->cameraPos, scaledFront, camera->cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vec3 scaledFront;
    glm_vec3_scale(camera->cameraFront, camera->cameraSpeed, scaledFront);
    glm_vec3_sub(camera->cameraPos, scaledFront, camera->cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    vec3 cross;
    glm_cross(camera->cameraFront, camera->cameraUp, cross);
    glm_vec3_normalize(cross);
    vec3 multiply;
    glm_vec3_scale(cross, camera->cameraSpeed, multiply);
    glm_vec3_sub(camera->cameraPos, multiply, camera->cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    vec3 cross;
    glm_cross(camera->cameraFront, camera->cameraUp, cross);
    glm_vec3_normalize(cross);
    vec3 multiply;
    glm_vec3_scale(cross, camera->cameraSpeed, multiply);
    glm_vec3_add(camera->cameraPos, multiply, camera->cameraPos);
  }
}
