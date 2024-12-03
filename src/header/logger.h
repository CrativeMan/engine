#ifndef LOGGER_H
#define LOGGER_H

#include <GL/gl.h>

void logToFile(const char *message, ...);
void loggerInfo(const char *id, const char *message, ...);
void loggerWarn(const char *id, const char *message, ...);
void loggerError(const char *id, const char *message, ...);
GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif // LOGGER_H
