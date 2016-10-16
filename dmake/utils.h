#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* strstrip(char* string);
char* strjoin(char** strings, size_t strings_len, char* delimiter);

#endif
