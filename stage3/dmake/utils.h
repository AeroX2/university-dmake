#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

char* strstrip(char* string, char* strip);
char* strjoin(char** strings, size_t strings_len, char* delimiter);
bool strfind(char* string, size_t length, int (*f)(int));

size_t hash(char* string);
size_t filehash(char* filename);

bool filecmp(char* file1, char* file2);
#endif
