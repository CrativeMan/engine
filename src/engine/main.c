#include <GL/glew.h>

#include "../header/callbacks.h"
#include "../header/logger.h"
#include "../header/main.h"
#include "../header/renderer.h"

/*** Defines ***/
#define ID "Engine"

/*** Global Variables ***/
Global global;
bool firstMouse = true;

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

  initCamera(&global.camera);

  // create shader
  global.shader[0] =
      createShader("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");
  global.shader[1] =
      createShader("src/shaders/vLight.glsl", "src/shaders/fLight.glsl");
  glCheckError();

  useShader(global.shader[0].id);
  shaderSetInt(global.shader[0].id, "material.diffuse", 0);
  shaderSetInt(global.shader[0].id, "material.specular", 1);
  glCheckError();

  loggerInfo(ID, "Initialized game engine");
}

/*** Shutdown functions ***/
void shutdown() {
  loggerInfo(ID, "Shutting down engine");
  for (int i = 0; i < 2; i++) {
    glDeleteProgram(global.shader[i].id);
    loggerInfo("Shader", "Deleted shader '%d'", global.shader[i].id);
  }
  loggerInfo(ID, "Exit engine");
  glfwTerminate();
  logToFile("--------------------------------------------");
}

int main() {
  init();

  Model *model = loadModel("resources/model/backpack/backpack.obj");

  loggerInfo(ID, "Started game loop");
  // main loop
  while (!glfwWindowShouldClose(global.window.id)) {
    glCheckError();
    inputCallback(global.window.id, &global.camera);
    render(&global.camera, &global.window, global.shader);
    glfwSwapBuffers(global.window.id);
    glfwPollEvents();
  }
  loggerInfo(ID, "Ended game loop");
  destroyModel(model);
  shutdown();
  return 0;
}
