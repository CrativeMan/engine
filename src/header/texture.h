#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct {
  unsigned int id;
  int width;
  int height;
  int nrComponents;
} Texture;

Texture loadTexture(char const *path);

#endif // TEXTURE_H
