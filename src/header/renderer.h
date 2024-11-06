#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "main.h"
#include "mesh.h"

void render(Mesh *mesh, Camera *camera, Window *window,
            unsigned int *shaderProgram, vec3 *cubePositions);
#endif // RENDERER_H
