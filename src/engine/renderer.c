#include <GL/glew.h>
#include <stdio.h>

#include "../header/logger.h"
#include "../header/renderer.h"
#include "../header/shader.h"
#include "GL/gl.h"
#include "cglm/types.h"

#define ID "Renderer"
#define X 0
#define Y 1
#define Z 2

vec3 cubePositions[] = {
    {0.0f, 0.0f, 0.0f},     {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f}, {-1.7f, 3.0f, -7.5f},
    {1.3f, -2.0f, -2.5f},   {1.5f, 2.0f, -2.5f},  {1.5f, 0.2f, -1.5f},
    {-1.3f, 1.0f, -1.5f},
};

void render(Mesh mesh[], Camera *camera, Window *window, Shader shader[]) {
  glCheckError();
  // setup delta time
  float currentFrame = glfwGetTime();
  camera->deltaTime = currentFrame - camera->lastFrame;
  camera->lastFrame = currentFrame;
  // draw background
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  // color bit for background depth for depth lol
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader[0].id);
  shaderSetVec3(shader[0].id, "light.direction", (vec3){-0.2f, -1.0f, -0.3f});
  shaderSetVec3(shader[0].id, "viewPos", camera->cameraPos);

  // light
  shaderSetVec3(shader[0].id, "light.ambient", (vec3){0.2f, 0.2f, 0.2f});
  shaderSetVec3(shader[0].id, "light.diffuse", (vec3){0.5f, 0.5f, 0.5f});
  shaderSetVec3(shader[0].id, "light.specular", (vec3){1.0f, 1.0f, 1.0f});

  // set material
  shaderSetFloat(shader[0].id, "material.shininess", 32.0f);

  // view/projection marix
  mat4 projection;
  glm_mat4_identity(projection);
  glm_perspective(glm_rad(camera->fov), 800.0f / 600.0f, 0.1f, 100.0f,
                  projection);
  mat4 view;
  vec3 center;
  glm_vec3_add(camera->cameraPos, camera->cameraFront, center);
  glm_lookat(camera->cameraPos, center, camera->cameraUp, view);
  shaderSetMat4(shader[0].id, "view", (float *)view);
  shaderSetMat4(shader[0].id, "projection", (float *)projection);

  // direction
  vec3 direction;
  direction[X] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  direction[Y] = sin(glm_rad(camera->pitch));
  direction[Z] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  glm_vec3_normalize_to(direction, camera->cameraFront);

  // set deffuse map
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mesh[0].texture[0].id);
  // set specular map
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, mesh[0].texture[1].id);

  // render cube
  mat4 model;
  glBindVertexArray(mesh[0].VAO);
  for (unsigned int i = 0; i < 10; i++) {
    glm_mat4_identity(model);
    glm_translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
    shaderSetMat4(shader[0].id, "model", (float *)model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

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
