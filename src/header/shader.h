#ifndef SHADER_H
#define SHADER_H

unsigned int createShader(char *vShaderPath, char *fShaderPath);
void shaderSetFloat(unsigned int id, char *uniform, float value);
void shaderSetInt(unsigned int id, char *uniform, int value);
void shaderSetMat4(unsigned int id, char *uniform, float *value);

#endif // SHADER_H
