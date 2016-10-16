#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char* strstrip(char* string, char* strip);
char* strjoin(char** strings, size_t strings_len, char* delimiter);

bool strfind(char* string, size_t length, int (*f)(int));

#endif
