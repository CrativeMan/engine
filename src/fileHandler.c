#include <stdio.h>
#include <stdlib.h>

#include "header/fileHandler.h"
#include "header/logger.h"

#define ID "FILE"

const char *readFile(const char *filepath) {
  FILE *file = fopen(filepath, "rb");
  if (file == NULL) {
    loggerError(ID, "Failed to open file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(fileSize + 1);
  if (buffer == NULL) {
    loggerError(ID, "Failed to alloc buffer");
    fclose(file);
    return NULL;
  }

  size_t readSize = fread(buffer, 1, fileSize, file);
  if (readSize != fileSize) {
    loggerError(ID, "Failed to read file");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[fileSize] = '\0';
  fclose(file);
  const char *fileContents = (const char *)buffer;

  return fileContents;
}
