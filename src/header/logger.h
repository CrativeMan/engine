#ifndef LOGGER_H
#define LOGGER_H

#include <GL/gl.h>

static const char *RESET = "\033[0m";
static const char *RED = "\033[0;31m";
static const char *YELLOW = "\033[0;33m";
static const char *GREEN = "\033[0;32m";
static const char *CYAN = "\033[0;36m";

void logToFile(const char *message, ...);
void logNotToFile(const char *id, const char *message, ...);
void loggerDev(const char *message, ...);
void loggerInfo(const char *id, const char *message, ...);
void loggerWarn(const char *id, const char *message, ...);
void loggerError(const char *id, const char *message, ...);
GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif // LOGGER_H
