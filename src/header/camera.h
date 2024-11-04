#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <cglm/vec3.h>

typedef struct {
  vec3 cameraPos;
  vec3 cameraFront;
  vec3 cameraUp;
  float cameraSpeed;
  float fov;
  float deltaTime;
  float lastFrame;
  float yaw;
  float pitch;
  float lastX;
  float lastY;
  float sensitivity;
} Camera;

void cameraProcessInput(GLFWwindow *window, Camera *camera);
void cameraProcessScrollInput(GLFWwindow *window, Camera *camera,
                              float yoffset);

#endif // CAMERA_H
