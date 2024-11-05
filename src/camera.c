#include "header/camera.h"
#include "header/logger.h"
#include <GLFW/glfw3.h>

void render(Camera *camera) {
  // setup delta time
  float currentFrame = glfwGetTime();
  camera->deltaTime = currentFrame - camera->lastFrame;
  camera->lastFrame = currentFrame;
}

void initCamera(Camera *camera) {
  glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera->cameraPos);
  glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->cameraFront);
  glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->cameraUp);
  camera->cameraSpeed = 0.05f;
  camera->fov = 45.0f;
  camera->yaw = -90.0f;
  camera->lastX = 400;
  camera->lastX = 300;
  camera->sensitivity = 0.1f;

  loggerInfo("Camera", "Initiated camera");
}

void cameraProcessScrollInput(Camera *camera, float yoffset) {
  camera->fov -= yoffset;
  if (camera->fov < 1.0f)
    camera->fov = 1.0f;
  if (camera->fov > 45.0f)
    camera->fov = 45.0f;
}

void cameraProcessInput(GLFWwindow *window, Camera *camera) {
  camera->cameraSpeed = 2.5f * camera->deltaTime;
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
