#include <GL/glew.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "header/logger.h"

const char *RESET = "\033[0m";
const char *RED = "\033[0;31m";
const char *YELLOW = "\003[0;33m";
const char *GREEN = "\033[0;32m";
const char *CYAN = "\033[0;36m";

void _loggerPrintId(char *id);

void loggerInfo(char *id, const char *message, ...) {
  _loggerPrintId(id);

  // print message and args
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("\n");
}

void loggerWarn(char *id, const char *message, ...) {
  printf("%s", YELLOW);
  _loggerPrintId(id);

  // print message and args
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("%s\n", RESET);
}

void loggerError(char *id, const char *message, ...) {
  printf("%s", RED);
  _loggerPrintId(id);

  // print message and args
  va_list args;
  va_start(args, message);
  vprintf(message, args);
  va_end(args);
  printf("%s\n", RESET);
}

void _loggerPrintId(char *id) {
  time_t now = time(NULL);
  struct tm *time_info = localtime(&now);

  // Print the timestamp and ID
  printf("[%02d:%02d:%02d] [%s]\t", time_info->tm_hour, time_info->tm_min,
         time_info->tm_sec, id);
}
