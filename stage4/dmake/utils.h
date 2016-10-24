#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool safe_malloc(void** pointer, size_t size);
bool safe_realloc(void** pointer, size_t size);

char* strstrip(char* string, char* strip);
char* strjoin(char** strings, size_t strings_len, char* delimiter);

bool strfind(char* string, size_t length, int (*f)(int));

#endif
