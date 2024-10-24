#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "header/fileHandler.h"
#include "header/logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "header/stb_img.h"

unsigned int loadImage(char *filename) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // maybe set texture parameters
  int width, height, nrChannels;
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    loggerError("Image", "Failed to load texture");
  }

  stbi_image_free(data);
  return texture;
}

const char *readFile(const char *filepath) {
  FILE *file = fopen(filepath, "rb");
  if (file == NULL) {
    loggerError("File", "Failed to open file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(fileSize + 1);
  if (buffer == NULL) {
    loggerError("File", "Failed to alloc buffer");
    fclose(file);
    return NULL;
  }

  size_t readSize = fread(buffer, 1, fileSize, file);
  if (readSize != fileSize) {
    loggerError("File", "Failed to read file");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[fileSize] = '\0';
  fclose(file);
  const char *fileContents = (const char *)buffer;

  return fileContents;
}
