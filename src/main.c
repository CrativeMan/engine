#include <GL/glew.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <string.h>

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
  vec3 cameraPos;
  vec3 cameraFront;
  vec3 cameraUp;
  float cameraSpeed;
  float deltaTime;
  float lastFrame;
} Camera;

typedef struct {
  GLFWwindow *window;
  unsigned int shaderProgram;
  unsigned int textures[2]; // XXX temp
  Camera camera;
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
  (void)window;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  global.camera.cameraSpeed = 2.5f * global.camera.deltaTime;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    vec3 scaledFront;
    glm_vec3_scale(global.camera.cameraFront, global.camera.cameraSpeed,
                   scaledFront);
    glm_vec3_add(global.camera.cameraPos, scaledFront, global.camera.cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    vec3 scaledFront;
    glm_vec3_scale(global.camera.cameraFront, global.camera.cameraSpeed,
                   scaledFront);
    glm_vec3_sub(global.camera.cameraPos, scaledFront, global.camera.cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    vec3 cross;
    glm_cross(global.camera.cameraFront, global.camera.cameraUp, cross);
    glm_vec3_normalize(cross);
    vec3 multiply;
    glm_vec3_scale(cross, global.camera.cameraSpeed, multiply);
    glm_vec3_sub(global.camera.cameraPos, multiply, global.camera.cameraPos);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    vec3 cross;
    glm_cross(global.camera.cameraFront, global.camera.cameraUp, cross);
    glm_vec3_normalize(cross);
    vec3 multiply;
    glm_vec3_scale(cross, global.camera.cameraSpeed, multiply);
    glm_vec3_add(global.camera.cameraPos, multiply, global.camera.cameraPos);
  }
}

char title[100];
/*** Rendering Functions ***/
void render(Mesh *mesh) {
  float currentFrame = glfwGetTime();
  global.camera.deltaTime = currentFrame - global.camera.lastFrame;
  global.camera.lastFrame = currentFrame;
  // draw background
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // color bit for background depth for depth lol
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use specific texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, global.textures[0]);

  mat4 projection;
  glm_mat4_identity(projection);
  glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

  mat4 view;
  vec3 center;
  glm_vec3_add(global.camera.cameraPos, global.camera.cameraFront, center);
  glm_lookat(global.camera.cameraPos, center, global.camera.cameraUp, view);

  shaderSetMat4(global.shaderProgram, "view", (float *)view);
  shaderSetMat4(global.shaderProgram, "projection", (float *)projection);

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
  snprintf(title, sizeof(title), "X:%.2f Y:%.2f", global.camera.cameraPos[0],
           global.camera.cameraPos[2]);
  glfwSetWindowTitle(global.window, title);
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

  // init camera
  glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, global.camera.cameraPos);
  glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, global.camera.cameraFront);
  glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, global.camera.cameraUp);
  global.camera.cameraSpeed = 0.05f;
}

/*** Shutdown functions ***/
void shutdown(Mesh *mesh) {
  // cleanup
  glDeleteVertexArrays(1, &mesh->VAO);
  glDeleteBuffers(1, &mesh->VBO);
  glDeleteProgram(global.shaderProgram);

  loggerInfo(ID, "Exit engine");
  glfwTerminate();
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
  shutdown(&mesh);
  return 0;
}
