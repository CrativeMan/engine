#include <GL/glew.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "header/fileHandler.h"
#include "header/logger.h"
#include "header/main.h"
#include "header/shader.h"

#define ID "Engine"

/*** Structures ***/
typedef struct {
  GLFWwindow *window;
  unsigned int shaderProgram;
  unsigned int textures[2]; // XXX temp
} Global;
Global global;

typedef struct {
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
} Mesh;

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
  glClear(GL_COLOR_BUFFER_BIT);

  float timeValue = glfwGetTime();
  float sin = sinf(timeValue) / 2.0f + 0.5f;
  shaderSetFloat(global.shaderProgram, "sin", sin);

  // use specific texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, global.textures[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, global.textures[1]);
  glBindVertexArray(mesh->VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
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
  loggerInfo(ID, "Created shader and images");

  Mesh mesh;
  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

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
