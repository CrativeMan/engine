#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "main.h"
#include "shader.h"

void render(Mesh mesh[], Camera *camera, Window *window, Shader shader[]);
#endif // RENDERER_H
