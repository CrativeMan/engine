#include <stdio.h>

#include "header/logger.h"

void loggerInfo(char *id, char *message) {
  fprintf(stdout, "[Info] [%s] %s\n", id, message);
}

void loggerError(char *id, char *message) {
  fprintf(stderr, "[Error] [%s] %s\n", id, message);
}
