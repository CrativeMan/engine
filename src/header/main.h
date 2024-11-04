#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

typedef struct {
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  unsigned int indicesCount;
} Mesh;

typedef struct {
  vec3 cameraPos;
  vec3 cameraFront;
  vec3 cameraUp;
  float cameraSpeed;
  float deltaTime;
  float lastFrame;
  float yaw;
  float pitch;
  float lastX;
  float lastY;
  float sensitivity;
} Camera;

typedef struct {
  GLFWwindow *window;
  char title[100];
} Window;

typedef struct {
  unsigned int shaderProgram;
  unsigned int textures[2]; // XXX temp
  Window window;
  Camera camera;
} Global;
#endif // MAIN_H
