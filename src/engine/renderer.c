#include <GL/glew.h>
#include <stdio.h>

#include "../header/logger.h"
#include "../header/renderer.h"
#include "../header/shader.h"
#include "cglm/affine-pre.h"
#include "cglm/mat4.h"

#define ID "Renderer"
#define X 0
#define Y 1
#define Z 2

void debugRender(bool *debug) {
  if (*debug == true)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (*debug == false)
    glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
}

void render(Mesh mesh[], Camera *camera, Window *window,
            unsigned int *shaderProgram) {
  glCheckError();
  // setup delta time
  float currentFrame = glfwGetTime();
  camera->deltaTime = currentFrame - camera->lastFrame;
  camera->lastFrame = currentFrame;
  // draw background
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // color bit for background depth for depth lol
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // matrices for camera
  mat4 projection;
  glm_mat4_identity(projection);
  glm_perspective(glm_rad(camera->fov), 800.0f / 600.0f, 0.1f, 100.0f,
                  projection);

  // direction
  vec3 direction;
  direction[X] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  direction[Y] = sin(glm_rad(camera->pitch));
  direction[Z] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  glm_vec3_normalize_to(direction, camera->cameraFront);

  mat4 view;
  vec3 center;
  glm_vec3_add(camera->cameraPos, camera->cameraFront, center);
  glm_lookat(camera->cameraPos, center, camera->cameraUp, view);

  shaderSetMat4(*shaderProgram, "view", (float *)view);
  shaderSetMat4(*shaderProgram, "projection", (float *)projection);

  for (int m = 0; m < 2; m++) {
    // use mesh
    glBindVertexArray(mesh[m].VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh[m].EBO);

    mat4 model;
    glm_mat4_identity(model);
    // loop through all cubes
    if (m == 0) {
      glUseProgram(*shaderProgram);
      glm_translate(model, (vec3){-3.0f, 0.0f, 0.0f});
      shaderSetMat4(*shaderProgram, "model", (float *)model);
      glDrawElements(GL_TRIANGLES, mesh[m].indicesCount, GL_UNSIGNED_INT, 0);
    } else {
      glUseProgram(*shaderProgram);
      glm_translate(model, (vec3){0.0f, 0.0f, -3.0f});
      shaderSetMat4(*shaderProgram, "model", (float *)model);
      glDrawElements(GL_TRIANGLES, mesh[m].indicesCount, GL_UNSIGNED_INT, 0);
    }
  }

  // set title
  snprintf(window->title, sizeof(window->title),
           "X:%.2f Y:%.2f Z:%.2f FOV:%.0f", camera->cameraPos[X],
           camera->cameraPos[Y], camera->cameraPos[Z], camera->fov);
  glfwSetWindowTitle(window->id, window->title);
}
