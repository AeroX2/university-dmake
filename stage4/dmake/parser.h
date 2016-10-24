#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <fcntl.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "utils.h"
#include "array.h"

#define SUCCESS         0
#define MALLOC_FAIL     1
#define SYNTAX_COLON    2
#define SYNTAX_RULE     3
#define NO_RULE_TO_FIRE 4
#define FAILURE_FORK    5

#define AT_MODIFIER        1 << 1
#define DASH_MODIFIER      1 << 2
#define EQUALS_MODIFIER    1 << 3
#define STAR_MODIFIER      1 << 4
#define AMPERSAND_MODIFIER 1 << 5

typedef struct Command
{
	char* command;
} Command;

typedef struct Rule
{
	char* target;

	Array dependencies;
	Array commands;

} Rule;

int parse(FILE* file);
int order();
int execute();

void debug_stage1();
void debug_stage2();
//void debug_stage3();

void free_rules();

#endif
