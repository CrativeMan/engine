#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "main.h"

void debugRender(bool *debug);
void render(Mesh mesh[], Camera *camera, Window *window,
            unsigned int *shaderProgram);
#endif // RENDERER_H
