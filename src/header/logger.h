#ifndef LOGGER_H
#define LOGGER_H

void loggerInfo(char *id, const char *message, ...);
void loggerWarn(char *id, const char *message, ...);
void loggerError(char *id, const char *message, ...);

#endif // LOGGER_H
