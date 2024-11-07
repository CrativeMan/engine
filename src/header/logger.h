#ifndef LOGGER_H
#define LOGGER_H

#include <GL/gl.h>

void loggerInfo(char *id, const char *message, ...);
void loggerWarn(char *id, const char *message, ...);
void loggerError(char *id, const char *message, ...);
GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif // LOGGER_H
