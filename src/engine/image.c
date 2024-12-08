#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../header/image.h"
#include "../header/logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../header/stb_img.h"

Texture loadTexture(char const *path) {
  Texture texture;
  glGenTextures(1, &texture.id);

  unsigned char *data = stbi_load(path, &texture.width, &texture.height,
                                  &texture.nrComponents, 0);
  if (data) {
    GLenum format = GL_RGBA;
    if (texture.nrComponents == 1)
      format = GL_RED;
    else if (texture.nrComponents == 3)
      format = GL_RGB;
    else if (texture.nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0,
                 format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    loggerInfo("Image", "Loaded image from '%s'", path);
  } else {
    loggerError("Image", "Failed to load image from '%s'", path);
    stbi_image_free(data);
  }

  return texture;
}
