#include <GL/glew.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "header/fileHandler.h"
#include "header/logger.h"
#include "header/shader.h"

unsigned int createShader(char *vShaderPath, char *fShaderPath) {
  // vertex shader
  const char *vShaderSource = readFile(vShaderPath);
  if (vShaderSource == NULL) {
    exit(EXIT_FAILURE);
  }
  unsigned int vShader;
  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderSource, NULL);
  glCompileShader(vShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vShader, 512, NULL, infoLog);
    loggerError("vShader", "Vertex shader compilation failed\n");
    printf("%s\n", infoLog);
  }

  // fragment shader
  const char *fShaderSource = readFile(fShaderPath);
  if (fShaderSource == NULL) {
    exit(EXIT_FAILURE);
  }
  unsigned int fShader;
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderSource, NULL);
  glCompileShader(fShader);

  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fShader, 512, NULL, infoLog);
    loggerError("fShader", "Fragment shader compilation failed");
    printf("%s\n", infoLog);
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
    loggerError("ShaderProgram", "Shader linking failed");
    printf("%s\n", infoLog);
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  return shaderProgram;
}

void shaderSetInt(unsigned int id, char *uniform, int value) {
  glUniform1i(glGetUniformLocation(id, uniform), value);
}

void shaderSetFloat(unsigned int id, char *uniform, float value) {
  glUniform1f(glGetUniformLocation(id, uniform), value);
}
