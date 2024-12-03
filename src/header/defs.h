#ifndef DEFS_H
#define DEFS_H

#include <time.h>

#define GET_HUMAN_READABLE_TIME(buffer)                                        \
  do {                                                                         \
    time_t current_time = time(NULL);                                          \
    if (current_time == ((time_t) - 1)) {                                      \
      snprintf(buffer, sizeof(buffer), "Time error");                          \
    } else {                                                                   \
      struct tm *local_time = localtime(&current_time);                        \
      if (local_time == NULL) {                                                \
        snprintf(buffer, sizeof(buffer), "Localtime error");                   \
      } else {                                                                 \
        strftime(buffer, sizeof(buffer), "%H:%M:%S", local_time);              \
      }                                                                        \
    }                                                                          \
  } while (0)

#define WRITE_LOG(message, ...)                                                \
  do {                                                                         \
    char time_buffer[100];                                                     \
    GET_HUMAN_READABLE_TIME(time_buffer);                                      \
    FILE *file = fopen("logs/engine.log", "a");                                \
    if (file != NULL) {                                                        \
      fprintf(file, "[%s] %s:%d: ", time_buffer, __FILE__, __LINE__);          \
      fprintf(file, message, ##__VA_ARGS__);                                   \
      fprintf(file, "\n");                                                     \
      fclose(file);                                                            \
    }                                                                          \
  } while (0)

#endif // DEFS_H
