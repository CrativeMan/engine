#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

/***
 * Asserts that the expression is true, otherwise prints the message and exits
 * the program.
 * Good case is a 0
 * Bad case is a 1
 ***/
#define ASSERT(_e, ...)                                                        \
  if (_e) {                                                                    \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);                                                                   \
  }

#endif // UTILS_H
