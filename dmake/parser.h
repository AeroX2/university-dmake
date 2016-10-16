#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

int parse(FILE* file);

int execute();
void debug_stage1();
//void debug_stage2();

void free_rules();

#endif
