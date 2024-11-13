#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <GLFW/glfw3.h>

#include "../header/logger.h"

const char *RESET = "\033[0m";
const char *RED = "\033[0;31m";
const char *YELLOW = "\033[0;33m";
const char *GREEN = "\033[0;32m";
const char *CYAN = "\033[0;36m";

void _loggerPrintId(const char *id);

void loggerInfo(const char *id, const char *message, ...) {
  _loggerPrintId(id);

  // print message and args
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

void loggerWarn(const char *id, const char *message, ...) {
  printf("%s", YELLOW);
  _loggerPrintId(id);

  // print message and args
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("%s\n", RESET);
}

void loggerError(const char *id, const char *message, ...) {
  printf("%s", RED);
  _loggerPrintId(id);

  // print message and args
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("%s\n", RESET);
}

void _loggerPrintId(const char *id) {
  time_t now = time(NULL);
  struct tm *time_info = localtime(&now);

  // Print the timestamp and ID
  printf("[%02d:%02d:%02d] [%s]\t", time_info->tm_hour, time_info->tm_min,
         time_info->tm_sec, id);
}

GLenum glCheckError_(const char *file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    const char *error;
    switch (errorCode) {
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_STACK_OVERFLOW:
      error = "STACK_OVERFLOW";
      break;
    case GL_STACK_UNDERFLOW:
      error = "STACK_UNDERFLOW";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    default:
      error = "UNKNOWN_ERROR";
      break;
    }
    loggerError(file, "'%s' on line %d", error, line);
  }

  return errorCode;
}
