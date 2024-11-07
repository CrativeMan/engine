#include "../header/camera.h"
#include "../header/logger.h"

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
