#include <GL/glew.h>
#include <stdio.h>

#include "../header/logger.h"
#include "../header/renderer.h"
#include "../header/shader.h"
#include "GL/gl.h"

#define ID "Renderer"
#define X 0
#define Y 1
#define Z 2

vec3 lightPos = {1.2f, 1.0f, 2.0f};

void render(Mesh mesh[], Camera *camera, Window *window, Shader shader[]) {
  glCheckError();
  // setup delta time
  float currentFrame = glfwGetTime();
  camera->deltaTime = currentFrame - camera->lastFrame;
  camera->lastFrame = currentFrame;
  // draw background
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  // color bit for background depth for depth lol
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader[0].id);
  shaderSetVec3(shader[0].id, "objectColor", (float[3]){1.0f, 0.5f, 0.31f});
  shaderSetVec3(shader[0].id, "viewPos", camera->cameraPos);

  // set light
  vec3 lightColor = {1.0f, 1.0f, 1.0f};

  vec3 diffuseColor;
  glm_vec3_mul(lightColor, (vec3){0.5f, 0.5f, 0.5f}, diffuseColor);
  vec3 ambientColor;
  glm_vec3_mul(diffuseColor, (vec3){0.2f, 0.2f, 0.2f}, ambientColor);

  shaderSetVec3(shader[0].id, "light.position", lightPos);
  shaderSetVec3(shader[0].id, "light.ambient", ambientColor);
  shaderSetVec3(shader[0].id, "light.diffuse", diffuseColor);
  shaderSetVec3(shader[0].id, "light.specular", (vec3){1.0f, 1.0f, 1.0f});

  // set material
  shaderSetVec3(shader[0].id, "material.specular",
                (float[3]){0.5f, 0.5f, 0.5f});
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

  // world transformation
  mat4 model;
  glm_mat4_identity(model);
  shaderSetMat4(shader[0].id, "model", (float *)model);

  // direction
  vec3 direction;
  direction[X] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  direction[Y] = sin(glm_rad(camera->pitch));
  direction[Z] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
  glm_vec3_normalize_to(direction, camera->cameraFront);

  // set deffuse map
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mesh[0].texture.id);

  // render cube
  glBindVertexArray(mesh[0].VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  /*** Light cube ***/
  glUseProgram(shader[1].id);
  shaderSetMat4(shader[1].id, "projection", (float *)projection);
  shaderSetMat4(shader[1].id, "view", (float *)view);
  glm_mat4_identity(model);
  lightPos[Y] = sin(currentFrame / 2.0f) * 2.3f;
  glm_translate(model, lightPos);
  glm_scale(model, (vec3){0.2f, 0.2f, 0.2f});
  shaderSetMat4(shader[1].id, "model", (float *)model);

  shaderSetVec3(shader[1].id, "objectColor", diffuseColor);

  glBindVertexArray(mesh[1].VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // set title
  snprintf(window->title, sizeof(window->title),
           "X:%.2f Y:%.2f Z:%.2f FOV:%.0f", camera->cameraPos[X],
           camera->cameraPos[Y], camera->cameraPos[Z], camera->fov);
  glfwSetWindowTitle(window->id, window->title);
}
