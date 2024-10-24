#include <GL/glew.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "header/shader.h"

unsigned int createShader(const char *vShaderSource,
                          const char *fShaderSource) {
  unsigned int vShader;
  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderSource, NULL);
  glCompileShader(vShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vShader, 512, NULL, infoLog);
    fprintf(stderr, "[Error] [vShader] Vertex shader compilation failed\n%s\n",
            infoLog);
  }

  unsigned int fShader;
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderSource, NULL);
  glCompileShader(fShader);

  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fShader, 512, NULL, infoLog);
    fprintf(stderr,
            "[Error] [fShader] Fragment shader compilation failed\n%s\n",
            infoLog);
  }

  // shader linking
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vShader);
  glAttachShader(shaderProgram, fShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "[Error] [shaderLinking] Shader linking failed\n%s\n",
            infoLog);
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  return shaderProgram;
}
