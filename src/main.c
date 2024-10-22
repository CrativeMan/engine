#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

/* defines */
#define WIN_WIDTH 512
#define WIN_HEIGHT 512

#define GRID_COUNT_X 8
#define GRID_COUNT_Y 8

/* globals */
GLfloat vertices[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
GLuint indices[] = {0, 1, 2, 2, 3, 0};

const GLchar *vShader_Code = "#version 330 core\n"
                             "layout (location = 0) in vec2 v_pos;\n"
                             "void main(){\n"
                             "  gl_Position = vec4(v_pos, 0.0, 1.0);\n"
                             "}\0";

const GLchar *fShader_Code = "#version 330 core\n"
                             "out vec4 fragment_color;\n"
                             "void main(){\n"
                             "  fragment_color = vec4(1.0, 0.5, 0.5, 1.0);\n"
                             "}\0";

struct Button {
  GLboolean down, last, pressed;
};

struct Keyboard {
  struct Button keys[GLFW_KEY_LAST];
};
struct Keyboard keyboard;

enum Directions { LEFT = 1, RIGHT = 2, UP = 4, DOWN = 8, PAUSE = 16 };

struct Snake {
  GLuint counter;
  GLuint positions[GRID_COUNT_X * GRID_COUNT_Y];
  GLuint direction;
};
struct Snake snake;

GLuint foodPosition = 0;

/* functions */
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key < 0)
    return;

  switch (action) {
  case GLFW_PRESS:
    keyboard.keys[key].down = GL_TRUE;
    break;
  case GLFW_RELEASE:
    keyboard.keys[key].down = GL_FALSE;
    break;
  default:
    break;
  }
}

void _button_array_update(struct Button *buttons) {
  int i;
  for (i = 0; i < GLFW_KEY_LAST; i++) {
    buttons[i].pressed = buttons[i].down && !buttons[i].last;
    buttons[i].last = buttons[i].down;
  }
}

void key_functions(GLFWwindow *window) {
  _button_array_update(keyboard.keys);

  if (keyboard.keys[GLFW_KEY_ESCAPE].down)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

/* main */
int main() {

  if (!glfwInit()) {
    fprintf(stderr, "[ERROR]: Failed to init GLFW!\n");
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "snake_clone", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "[ERROR]: Failed creating GLFW window!\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "[ERROR]: Failed to init GLEW\n");
    exit(EXIT_FAILURE);
  }

  GLuint vShader, fShader, shader_program;
  GLsizei maxLength = 200, length;
  GLchar infoLog[maxLength];
  GLint success;
  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShader_Code, NULL);
  glCompileShader(vShader);

  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vShader, maxLength, &length, infoLog);
    fprintf(stderr, "[ERROR] [SHADER](vertex) compilation failed\n%s\n]",
            infoLog);
  }

  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShader_Code, NULL);
  glCompileShader(fShader);

  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fShader, maxLength, &length, infoLog);
    fprintf(stderr, "[ERROR] [SHADER(fragment) compilation failed\n%s\n]",
            infoLog);
  }

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vShader);
  glAttachShader(shader_program, fShader);
  glLinkProgram(shader_program);
  glDeleteShader(vShader);
  glDeleteShader(fShader);

  GLuint vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    key_functions(window);

    glBindVertexArray(vao);
    glUseProgram(shader_program);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
