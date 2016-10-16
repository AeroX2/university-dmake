#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "utils.h"

int parse(FILE* file);

int execute();
void debug_stage1();
//void debug_stage2();

char* strjoin(char** strings, size_t strings_len, char* delimiter);

#endif
