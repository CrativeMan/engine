#include <GL/glew.h>

#include "../header/callbacks.h"
#include "../header/logger.h"
#include "../header/main.h"
#include "../header/renderer.h"
#include "GL/glext.h"

/*** Defines ***/
#define ID "Engine"

/*** Global Variables ***/
Global global;
bool firstMouse;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, // 1
    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, // 2
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, // 3
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, // 4
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, // 5
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, // 6

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, // 7
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, // 8
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 9
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 10
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 11
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, // 12

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, // 13
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f, // 14
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, // 15
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, // 16
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f, // 17
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, // 18

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 19
    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f, // 20
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // 21
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, // 22
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, // 23
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, // 24

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, // 25
    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, // 26
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, // 27
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, // 28
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, // 29
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, // 30

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, // 31
    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, // 32
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 33
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 34
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f, // 35
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f  // 36
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
  global.counter = 0;
  if (!glfwInit()) {
    loggerError("GLFW", "Failed to init glfw");
    exit(EXIT_FAILURE);
  }
  // set opengl version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  global.window.id =
      glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "engine", NULL, NULL);
  if (global.window.id == NULL) {
    loggerError("GLFW", "Failed to create window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(global.window.id);
  loggerInfo(ID, "Generated window %d", global.window.id);

  glfwSetInputMode(global.window.id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(global.window.id, mousePosCallback);
  glfwSetScrollCallback(global.window.id, mouseScrollCallback);

  // init glew
  glewExperimental = GL_TRUE;
  glewInit();

  // resize stuff
  glfwSetFramebufferSizeCallback(global.window.id, frame_buffer_size_callback);
  glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

  // enable depth
  glEnable(GL_DEPTH_TEST);

  // create shader
  global.shader[0] =
      createShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  global.shader[1] =
      createShader("src/shaders/vLight.glsl", "src/shaders/fLight.glsl");

  initCamera(&global.camera);

  // init meshes
  // initializeMesh(&global.mesh[0], vertices, sizeof(vertices),
  // &global.counter); initializeMesh(&global.mesh[1], vertices,
  // sizeof(vertices), &global.counter);
  glCheckError();

  // enable shader

  loggerInfo(ID, "Initialized game engine");
}

/*** Shutdown functions ***/
void shutdown() {
  loggerInfo(ID, "Shutting down engine");
  // cleanup
  for (int i = 0; i < 2; i++) {
    deleteMesh(&global.mesh[i]);
    glDeleteProgram(global.shader[i].id);
  }

  loggerInfo(ID, "Exit engine");
  logToFile("--------------------------------------------");

  glfwTerminate();
}

int main() {
  init();

  tempMeshes(global.mesh, vertices, sizeof(vertices), (int[]){0, 1});

  loggerInfo(ID, "Started game loop");
  // main loop
  while (!glfwWindowShouldClose(global.window.id)) {
    glCheckError();
    // input
    processInput(global.window.id);

    // rendering
    render(global.mesh, &global.camera, &global.window, global.shader);

    // check all events and swap buffers
    glfwSwapBuffers(global.window.id);
    glfwPollEvents();
  }
  loggerInfo(ID, "Ended game loop");
  shutdown();
  return 0;
}
