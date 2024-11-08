#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include "../header/callbacks.h"
#include "../header/logger.h"
#include "../header/main.h"
#include "../header/renderer.h"
#include "../header/shader.h"

/*** Defines ***/
#define ID "Engine"

/*** Global Variables ***/
Global global;
Mesh mesh;
bool firstMouse;

float vertices[] = {
    // pos
    -0.5f, -0.5f, -0.5f, // 0
    0.5f,  -0.5f, -0.5f, // 1
    0.5f,  -0.5f, 0.5f,  // 2
    -0.5f, -0.5f, 0.5f,  // 3
    -0.5f, 0.5f,  -0.5f, // 4
    0.5f,  0.5f,  -0.5f, // 5
    0.5f,  0.5f,  0.5f,  // 6
    -0.5f, 0.5f,  0.5f,  // 7
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
  scrollCallback(&global.camera, yoffset);
}

void mousePosCallback(GLFWwindow *window, double xpos, double ypos) {
  (void)window;
  mouseCallback(&global.camera, xpos, ypos, &firstMouse);
}

void processInput(GLFWwindow *window) {
  inputCallback(window, &global.camera, &global.debug);
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

  // create window
  global.window.windowId =
      glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "engine", NULL, NULL);
  if (global.window.windowId == NULL) {
    loggerError("GLFW", "Failed to create window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(global.window.windowId);
  loggerInfo(ID, "Generated window %d", global.window.windowId);

  glfwSetInputMode(global.window.windowId, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(global.window.windowId, mousePosCallback);
  glfwSetScrollCallback(global.window.windowId, mouseScrollCallback);

  // init glew
  glewExperimental = GL_TRUE;
  glewInit();

  // resize stuff
  glfwSetFramebufferSizeCallback(global.window.windowId,
                                 frame_buffer_size_callback);
  glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

  // enable depth
  glEnable(GL_DEPTH_TEST);

  // create shader
  global.shaderProgram =
      createShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

  initCamera(&global.camera);
  char *texture[] = {"src/textures/wall.jpg", "src/textures/awesomeface.png"};
  initializeMesh(&mesh, vertices, sizeof(vertices), indices, sizeof(indices));

  // enable shader
  glUseProgram(global.shaderProgram);
  glUniform1i(glGetUniformLocation(global.shaderProgram, "texture1"), 0);
  shaderSetInt(global.shaderProgram, "texture2", 1);

  glCheckError();
  loggerInfo(ID, "Initialized game engine");
}

/*** Shutdown functions ***/
void shutdown(Mesh *mesh) {
  // cleanup
  glDeleteVertexArrays(1, &mesh->VAO);
  glDeleteBuffers(1, &mesh->VBO);
  glDeleteProgram(global.shaderProgram);
  deleteMesh(mesh);

  loggerInfo(ID, "Exit engine");
  glfwTerminate();
}

int main() {
  init();

  loggerInfo(ID, "Started game loop");
  // main loop
  while (!glfwWindowShouldClose(global.window.windowId)) {
    // input
    processInput(global.window.windowId);

    // rendering
    render(&mesh, &global.camera, &global.window, &global.shaderProgram,
           cubePositions);
    debugRender(&global.debug);

    // check all events and swap buffers
    glfwSwapBuffers(global.window.windowId);
    glfwPollEvents();
  }
  shutdown(&mesh);
  return 0;
}
