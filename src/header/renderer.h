#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "main.h"
#include "model.h"
#include "shader.h"

void render(Model *model, Camera *camera, Window *window, Shader shader[]);
#endif // RENDERER_H
