#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <time.h>
#include <fcntl.h>
#include <utime.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#include "utils.h"
#include "array.h"
#include "hashtable.h"

#define SUCCESS         0
#define SYNTAX_COLON    1
#define SYNTAX_RULE     2
#define NO_RULE_TO_FIRE 3
#define MODIFIER        4
#define FAILURE_FORK    5
#define INTERRUPT       6
#define NUMBER_FORMAT   7

#define AT_MODIFIER        1 << 1
#define DASH_MODIFIER      1 << 2
#define EQUALS_MODIFIER    1 << 3
#define STAR_MODIFIER      1 << 4
#define AMPERSAND_MODIFIER 1 << 5
#define SECONDS_MODIFIER   1 << 6

volatile extern bool terminate;

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

typedef struct File
{
	size_t hash;
	time_t old_time;
	time_t new_time;
} File;

int parse(FILE* file);
int order();
int execute(bool debug);
void execute_modifiers(char* command, int* modifiers, char** times, int* offset);
int execute_command(char* command, int modifiers, char** times, int offset);

void debug_stage1();
void debug_stage2();
//void debug_stage3();

void free_rules();

void child_signal_handler();

#endif
