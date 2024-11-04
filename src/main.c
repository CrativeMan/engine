#include <GL/glew.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <string.h>

#include "cglm/types.h"
#include "header/callbacks.h"
#include "header/camera.h"
#include "header/fileHandler.h"
#include "header/logger.h"
#include "header/main.h"
#include "header/shader.h"

/*** Defines ***/
#define ID "Engine"
#define X 0
#define Y 1
#define Z 2

typedef struct {
  unsigned int shaderProgram;
  Window window;
  Camera camera;
} Global;
Global global;
Mesh mesh;
bool firstMouse;

/*** Global Variables ***/
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

/*** Callback Functions ***/
void mouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  (void)window;
  (void)xoffset;
  cameraProcessScrollInput(&global.camera, yoffset);
}

void mousePosCallback(GLFWwindow *window, double xpos, double ypos) {
  (void)window;

  if (firstMouse) {
    global.camera.lastX = xpos;
    global.camera.lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - global.camera.lastX;
  float yoffset = global.camera.lastY - ypos;

  global.camera.lastX = xpos;
  global.camera.lastY = ypos;
  xoffset *= global.camera.sensitivity;
  yoffset *= global.camera.sensitivity;

  global.camera.yaw += xoffset;
  global.camera.pitch += yoffset;

  if (global.camera.pitch > 89.0f)
    global.camera.pitch = 89.0f;
  if (global.camera.pitch < -89.0f)
    global.camera.pitch = -89.0f;
}

/*** Input functions ***/
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  cameraProcessInput(window, &global.camera);
}

/*** Rendering Functions ***/
void renderFrame(Mesh *mesh) {
  // setup delta time
  float currentFrame = glfwGetTime();
  global.camera.deltaTime = currentFrame - global.camera.lastFrame;
  global.camera.lastFrame = currentFrame;

  // draw background
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // color bit for background depth for depth lol
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use specific texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mesh->texture);

  // matrices for camera
  mat4 projection;
  glm_mat4_identity(projection);
  glm_perspective(glm_rad(global.camera.fov), 800.0f / 600.0f, 0.1f, 100.0f,
                  projection);

  // direction
  vec3 direction;
  direction[0] =
      cos(glm_rad(global.camera.yaw)) * cos(glm_rad(global.camera.pitch));
  direction[1] = sin(glm_rad(global.camera.pitch));
  direction[2] =
      sin(glm_rad(global.camera.yaw)) * cos(glm_rad(global.camera.pitch));
  glm_vec3_normalize_to(direction, global.camera.cameraFront);

  mat4 view;
  vec3 center;
  glm_vec3_add(global.camera.cameraPos, global.camera.cameraFront, center);
  glm_lookat(global.camera.cameraPos, center, global.camera.cameraUp, view);

  shaderSetMat4(global.shaderProgram, "view", (float *)view);
  shaderSetMat4(global.shaderProgram, "projection", (float *)projection);

  // use mesh
  glBindVertexArray(mesh->VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
  // loop through all cubes
  for (int i = 0; i < 10; i++) {
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

  // set title
  snprintf(global.window.title, sizeof(global.window.title),
           "X:%.2f Y:%.2f Z:%.2f FOV:%.0f", global.camera.cameraPos[X],
           global.camera.cameraPos[Y], global.camera.cameraPos[Z],
           global.camera.fov);
  glfwSetWindowTitle(global.window.window, global.window.title);
}

/*** Init functions ***/
void init(Mesh *mesh) {
  if (!glfwInit()) {
    loggerError("GLFW", "Failed to init glfw");
    exit(EXIT_FAILURE);
  }
  // set opengl version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  global.window.window =
      glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "engine", NULL, NULL);
  if (global.window.window == NULL) {
    loggerError("GLFW", "Failed to create window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(global.window.window);

  glfwSetInputMode(global.window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(global.window.window, mousePosCallback);
  glfwSetScrollCallback(global.window.window, mouseScrollCallback);

  // init glew
  glewExperimental = GL_TRUE;
  glewInit();

  // resize stuff
  glfwSetFramebufferSizeCallback(global.window.window,
                                 frame_buffer_size_callback);
  glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

  // enable depth
  glEnable(GL_DEPTH_TEST);

  // create shader
  global.shaderProgram =
      createShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  // texture
  mesh->texture = loadImage("src/textures/wall.jpg");

  loggerInfo(ID, "Initialized game engine");
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
  // if draw in wireframe
  if (argc == 2)
    if (strcmp(argv[1], " -w"))
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  init(&mesh);
  initCamera(&global.camera);
  // init mesh
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
  mesh.verticesCount = 40;
  mesh.indicesCount = 36;
  mesh.cubePosCount = 10;

  // enable shader
  glUseProgram(global.shaderProgram);
  glUniform1i(glGetUniformLocation(global.shaderProgram, "texture1"), 0);
  shaderSetInt(global.shaderProgram, "texture2", 1);

  loggerInfo(ID, "Started game loop");
  // main loop
  while (!glfwWindowShouldClose(global.window.window)) {
    // input
    processInput(global.window.window);

    // rendering
    renderFrame(&mesh);

    // check all events and swap buffers
    glfwSwapBuffers(global.window.window);
    glfwPollEvents();
  }
  shutdown(&mesh);
  return 0;
}
