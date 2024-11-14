#include <GL/glew.h>

#include <GL/gl.h>
#include <string.h>

#include "../header/callbacks.h"
#include "../header/logger.h"
#include "../header/main.h"
#include "../header/renderer.h"

/*** Defines ***/
#define ID "Engine"

/*** Global Variables ***/
Global global;
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
  global.shader =
      createShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

  initCamera(&global.camera);

  // init meshes
  initializeMesh(&global.mesh[0], vertices, sizeof(vertices), indices,
                 sizeof(indices), &global.counter);
  initializeMesh(&global.mesh[1], vertices, sizeof(vertices), indices,
                 sizeof(indices), &global.counter);
  glCheckError();

  // enable shader

  loggerInfo(ID, "Initialized game engine");
}

/*** Shutdown functions ***/
void shutdown() {
  loggerInfo(ID, "Shutting down engine");
  // cleanup
  glDeleteProgram(global.shader.id);
  for (int i = 0; i < 2; i++) {
    deleteMesh(&global.mesh[i]);
  }

  loggerInfo(ID, "Exit engine");
  glfwTerminate();
}

int main() {
  init();

  loggerInfo(ID, "Started game loop");
  // main loop
  while (!glfwWindowShouldClose(global.window.id)) {
    glCheckError();
    // input
    processInput(global.window.id);

    // rendering
    render(global.mesh, &global.camera, &global.window, &global.shader.id);

    // check all events and swap buffers
    glfwSwapBuffers(global.window.id);
    glfwPollEvents();
  }
  loggerInfo(ID, "Ended game loop");
  shutdown();
  return 0;
}
