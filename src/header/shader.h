#ifndef SHADER_H
#define SHADER_H

enum { SHADER_VERTEX, SHADER_FRAGMENT, SHADER_PROGRAM };

typedef struct {
  unsigned int id;
  char type;
} Shader;

Shader createShader(char *vShaderPath, char *fShaderPath);
void shaderSetFloat(unsigned int id, char *uniform, float value);
void shaderSetInt(unsigned int id, char *uniform, int value);
void shaderSetVec3(unsigned int id, char *uniform, float value[3]);
void shaderSetMat4(unsigned int id, char *uniform, float *value);

#endif // SHADER_H
