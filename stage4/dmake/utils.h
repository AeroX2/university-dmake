#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* strstrip(char* string, char* strip);
char* strjoin(char** strings, size_t strings_len, char* delimiter);
bool strfind(char* string, size_t length, int (*f)(int));

size_t hash(char* string);
size_t filehash(char* filename);

size_t count_digits(long long number);
#endif
