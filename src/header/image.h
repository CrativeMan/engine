#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
  unsigned int id;
  int width;
  int height;
  int nrComponents;
} Texture;

Texture loadTexture(char const *path);

#endif // IMAGE_H
