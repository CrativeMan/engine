#include <GL/glew.h>
#include <stdio.h>

#include "header/logger.h"

const char *RESET = "\033[0m";
const char *RED = "\033[0;31m";
const char *GREEN = "\033[0;32m";
const char *CYAN = "\033[0;36m";

void loggerInfo(char *id, char *message) {
  fprintf(stdout, "%s[Info] [%s] %s%s\n", CYAN, id, message, RESET);
}

void loggerError(char *id, char *message) {
  fprintf(stderr, "%s[Error] [%s] %s%s\n", RED, id, message, RESET);
}
