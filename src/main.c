#include <GL/glew.h>
#include <math.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <string.h>

#include "cglm/affine-pre.h"
#include "cglm/affine.h"
#include "cglm/types.h"
#include "header/fileHandler.h"
#include "header/logger.h"
#include "header/main.h"
#include "header/shader.h"

#define ID "Engine"

/*** Structures ***/
typedef struct {
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
} Mesh;

typedef struct {
  GLFWwindow *window;
  unsigned int shaderProgram;
  unsigned int textures[2]; // XXX temp
  Mesh meshes[2];
} Global;
Global global;

/*** Function definitions ***/
void render(Mesh *mesh1, Mesh *mesh2);

/*** Callback Functions ***/
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/*** Rendering Functions ***/
void render(Mesh *mesh1, Mesh *mesh2) {
  // draw background
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // use specific texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, global.textures[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, global.textures[1]);

  // rotation and translation
  // order has to be in reverse because of matrix multiplication
  mat4 trans;
  vec3 translate = {0.5f, -0.5f, 0.0f};
  vec3 axis = {0.0f, 0.f, 1.0f};
  glm_mat4_identity(trans);
  glm_translate(trans, translate);
  glm_rotate(trans, (float)glfwGetTime(), axis);

  // set the transform and rotation mat4 to the shader
  shaderSetMat4(global.shaderProgram, "transform", (float *)trans);

  glBindVertexArray(mesh1->VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh1->EBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // other shader stuff
  vec3 translation = {-0.5f, 0.5f, 0.0f};
  float time = glfwGetTime();
  vec3 scale = {(float)(1.0 * sin(1.0 * time)), (float)(1.0 * sin(1.0 * time)),
                (float)(1.0 * sin(1.0 * time))};
  // vec3 scale = {0.5f, 0.5f, 0.5f};
  glm_mat4_identity(trans);
  glm_translate(trans, translation);
  glm_scale(trans, scale);

  shaderSetMat4(global.shaderProgram, "transform", (float *)trans);

  glBindVertexArray(mesh2->VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh2->EBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

/*** Init functions ***/
void init() {
  if (!glfwInit()) {
    loggerError("GLFW", "Failed to init glfw");
    exit(EXIT_FAILURE);
  }
  // set opengl version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  global.window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "engine", NULL, NULL);
  if (global.window == NULL) {
    loggerError("GLFW", "Failed to create window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(global.window);

  // init glew
  glewExperimental = GL_TRUE;
  glewInit();

  // resize stuff
  glfwSetFramebufferSizeCallback(global.window, frame_buffer_size_callback);
  glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

  loggerInfo(ID, "Initialized game engine");
}

int main(int argc, char **argv) {
  init();

  // create shader
  global.shaderProgram =
      createShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  global.textures[0] = loadImage("src/textures/wall.jpg");
  global.textures[1] = loadImage("src/textures/awesomeface.png");

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  // mesh 1
  glGenVertexArrays(1, &global.meshes[0].VAO);
  glGenBuffers(1, &global.meshes[0].VBO);
  glGenBuffers(1, &global.meshes[0].EBO);

  glBindVertexArray(global.meshes[0].VAO);

  glBindBuffer(GL_ARRAY_BUFFER, global.meshes[0].VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, global.meshes[0].EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // vertex attrib pointers
  // pointer to the position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // pointer to the color values
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // pointer to the tex coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // mesh 2
  global.meshes[1] = global.meshes[0];

  // if draw in wireframe
  if (argc == 2)
    if (strcmp(argv[1], " -w"))
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // enable shader
  glUseProgram(global.shaderProgram);
  glUniform1i(glGetUniformLocation(global.shaderProgram, "texture1"), 0);
  shaderSetInt(global.shaderProgram, "texture2", 1);

  loggerInfo(ID, "Started game loop");
  // main loop
  while (!glfwWindowShouldClose(global.window)) {
    // input
    processInput(global.window);

    // rendering
    render(&global.meshes[0], &global.meshes[1]);

    // check all events and swap buffers
    glfwSwapBuffers(global.window);
    glfwPollEvents();
  }

  // cleanup
  glDeleteVertexArrays(1, &global.meshes[0].VAO);
  glDeleteVertexArrays(1, &global.meshes[1].VAO);
  glDeleteBuffers(1, &global.meshes[0].VBO);
  glDeleteBuffers(1, &global.meshes[1].VBO);
  glDeleteProgram(global.shaderProgram);

  loggerInfo(ID, "Exit engine");
  glfwTerminate();
  return 0;
}
