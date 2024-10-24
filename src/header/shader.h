#ifndef SHADER_H
#define SHADER_H

unsigned int createShader(char *vShaderPath, char *fShaderPath);
void setFloat(unsigned int id, char *uniform, float value);

#endif // SHADER_H
