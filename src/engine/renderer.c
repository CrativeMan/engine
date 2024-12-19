#include <GL/glew.h>
#include <stdio.h>

#include "../header/logger.h"
#include "../header/renderer.h"
#include "../header/shader.h"

#define ID "Renderer"
#define X 0
#define Y 1
#define Z 2

#define VEC3(x) ((vec3){(x), (x), (x)})

void render(Mesh mesh[], Camera *camera, Window *window, Shader shader[]) {
  glCheckError();
  // setup delta time
  float currentFrame = glfwGetTime();
  camera->deltaTime = currentFrame - camera->lastFrame;
  camera->lastFrame = currentFrame;
  // draw background
  glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
  // color bit for background depth for depth lol
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // handle fps
  camera->fps = 1 / camera->deltaTime;
  static float fpsTimer = 0.0f;
  fpsTimer += camera->deltaTime;
  if (fpsTimer >= 1.0f) {
    logNotToFile(ID, "FPS: %.1f", camera->fps);
    fpsTimer = 0.0f;
  }

  // set title
  snprintf(window->title, sizeof(window->title),
           "X:%.2f Y:%.2f Z:%.2f FOV:%.0f", camera->cameraPos[X],
           camera->cameraPos[Y], camera->cameraPos[Z], camera->fov);
  glfwSetWindowTitle(window->id, window->title);
}
