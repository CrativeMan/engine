#include <GL/glew.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <string.h>

#include "cglm/vec3.h"
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
  unsigned int indicesCount;
} Mesh;

typedef struct {
  GLFWwindow *window;
  unsigned int shaderProgram;
  unsigned int textures[2]; // XXX temp
  vec3 cameraPos;
} Global;
Global global;

float vertices[] = {
    // pos              tex
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, // 1
    0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, // 2
    -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, // 3
    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, // 4
    0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, // 5
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f  // 7
};

unsigned int indices[] = {
    0, 1, 2, 0, 2, 3, // down
    4, 5, 6, 4, 6, 7, // up
    1, 2, 6, 1, 6, 5, // right
    3, 0, 4, 3, 4, 7, // left
    0, 4, 5, 0, 1, 5, // front
    3, 2, 6, 3, 6, 7, // back
};

vec3 cubePositions[] = {
    {0.0f, 0.0f, 0.0f},     {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f}, {-1.7f, 3.0f, -7.5f},
    {1.3f, -2.0f, -2.5f},   {1.5f, 2.0f, -2.5f},  {1.5f, 0.2f, -1.5f},
    {-1.3f, 1.0f, -1.5f},
};

/*** Function definitions ***/
void render(Mesh *mesh);

/*** Callback Functions ***/
void frame_buffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/*** Rendering Functions ***/
void render(Mesh *mesh) {
  // draw background
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // color bit for background depth for depth lol
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use specific texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, global.textures[0]);

  // view matrix
  mat4 view;
  glm_mat4_identity(view);
  glm_translate(view, (vec3){0.0f, 0.0f, -10.0f});

  mat4 projection;
  glm_mat4_identity(projection);
  glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

  shaderSetMat4(global.shaderProgram, "view", (float *)view);
  shaderSetMat4(global.shaderProgram, "projection", (float *)projection);

  vec3 cameraPos = (vec3){0.0f, 0.0f, 3.0f};
  vec3 cameraTarget = (vec3){0.0f, 0.0f, 0.0f};
  vec3 cameraDirection;
  glm_vec3_sub(cameraPos, cameraTarget, cameraDirection);
  glm_normalize(cameraDirection);

  vec3 up = (vec3){0.0f, 1.0f, 0.0f};
  vec3 cameraRight;
  glm_cross(up, cameraDirection, cameraRight);
  glm_normalize(cameraRight);

  vec3 cameraUp;
  glm_cross(cameraDirection, cameraRight, cameraUp);

  glBindVertexArray(mesh->VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
  for (unsigned int i = 0; i < 10; i++) {
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    if (i % 3 == 0)
      angle = glfwGetTime() * 25.0f;
    glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
    shaderSetMat4(global.shaderProgram, "model", (float *)model);
    glDrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);
  }
  // glDrawArrays(GL_TRIANGLES, 0, 36);
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

  glEnable(GL_DEPTH_TEST);

  // create shader
  global.shaderProgram =
      createShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  global.textures[0] = loadImage("src/textures/wall.jpg");

  loggerInfo(ID, "Initialized game engine");
}

int main(int argc, char **argv) {
  init();

  // mesh 1
  Mesh mesh;
  mesh.indicesCount = sizeof(indices) / sizeof(unsigned int);
  glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);
  glGenBuffers(1, &mesh.EBO);

  glBindVertexArray(mesh.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // vertex attrib pointers
  // pointer to the position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // pointer to the color values
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // pointer to the tex coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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
    render(&mesh);

    // check all events and swap buffers
    glfwSwapBuffers(global.window);
    glfwPollEvents();
  }

  // cleanup
  glDeleteVertexArrays(1, &mesh.VAO);
  glDeleteBuffers(1, &mesh.VBO);
  glDeleteProgram(global.shaderProgram);

  loggerInfo(ID, "Exit engine");
  glfwTerminate();
  return 0;
}
