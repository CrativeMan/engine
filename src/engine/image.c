#include <GL/glew.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../header/image.h"
#include "../header/logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../header/stb_img.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../header/stb_image_write.h"

#define ID "Image"

Image loadImage(char const *path){
  Image img;
  glGenTextures(1, &img.id);

  unsigned char *data = stbi_load(path, &img.width, &img.height,
                                  &img.nrComponents, 0);
  if (data) {
    GLenum format = GL_RGBA;
    if (img.nrComponents == 1)
      format = GL_RED;
    else if (img.nrComponents == 3)
      format = GL_RGB;
    else if (img.nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, img.id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, img.width, img.height, 0,
                 format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    loggerInfo(ID, "Loaded image from '%s'", path);
  } else {
    loggerError(ID, "Failed to load image from '%s'", path);
    stbi_image_free(data);
  }

  return img;
}

void saveFrameBufferToPng(GLFWwindow *window) {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  unsigned char *pixels = malloc(width * height * 3);
  if (!pixels) {
    loggerError(ID, "Failed to malloc mem for screenshot");
    return;
  }

  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

  time_t now;
  struct tm* timeinfo;
  char filename[256];

  time(&now);
  timeinfo = localtime(&now);

  strftime(filename, sizeof(filename), "logs/imgs/screenshot_%Y%m%d_%H%M%S.png", timeinfo);

  unsigned char *flipped_pixels = malloc(width * height * 3);
  if (!flipped_pixels) {
    loggerError(ID, "Failed to malloc mem for flipped_pixels");
    free(pixels);
    return;
  }

  int i;
  for (i = 0; i < height; i++) {
    memcpy(flipped_pixels + (i * width * 3), pixels+((height - i - 1) * width * 3), width * 3);
  }

  int result = stbi_write_png(
    filename,
    width,
    height,
    3,
    flipped_pixels,
    width * 3
  );

  free(pixels);
  free(flipped_pixels);

  if (result) {
    loggerInfo(ID, "Screenshot saved as %s", filename);
  } else {
    loggerError(ID, "Failed to save screenshot");
  }
}
