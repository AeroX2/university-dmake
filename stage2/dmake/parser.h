#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "utils.h"
#include "array.h"

#define SUCCESS 0
#define MALLOC_FAIL 1
#define SYNTAX_COLON 2
#define SYNTAX_RULE 3

int parse(FILE* file);
int order();

int execute();
void debug_stage1();
void debug_stage2();
//void debug_stage3();

void free_rules();

#endif
