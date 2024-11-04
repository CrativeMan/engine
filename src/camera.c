#include "header/camera.h"
#include <GLFW/glfw3.h>

void cameraProcessScrollInput(GLFWwindow *window, Camera *camera,
                              float yoffset) {
  camera->fov -= yoffset;
  if (camera->fov < 1.0f)
    camera->fov = 1.0f;
  if (camera->fov > 45.0f)
    camera->fov = 45.0f;
}

void cameraProcessInput(GLFWwindow *window, Camera *camera) {
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
