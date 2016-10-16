/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Sun 16 Oct 2016 15:06:56 AEDT
 */

#include "parser.h"

//TODO Dynamic array
typedef struct Command
{
	char* command;
} Command;

typedef struct Rule
{
	char* rule_name;

	char* files[1000];
	size_t files_size;

	Command commands[1000];
	size_t commands_size;
} Rule;

Rule rules[1000];
size_t rules_size = 0;

int parse(FILE* file)
{
	char* line = NULL;
	char* line_raw = NULL;
	size_t len = 0;

	bool append = false;

	while (getline(&line_raw, &len, file) != -1)
	{
		size_t length = line != NULL ? strlen(line) : 0;
		size_t length_raw = strlen(line_raw);

		if (append) 
		{
			line[length-2] = '\0';
			//Another strip but I don't want to strip newlines and I only want to strip if the line contains characters
			bool strip = false;
			for (size_t i = 0; i < length_raw; i++) if (isalnum(line_raw[i])) strip = true;
			if (strip) while (isblank(*++line_raw));

			line = realloc(line, length+length_raw-1);
			strcat(line, line_raw);

			append = false;
		}
		else 
		{
			if (length_raw > length) line = realloc(line, length_raw+1);
			strcpy(line, line_raw);
		}

		if (line[strlen(line)-2] == '\\') 
		{
			append = true;
			continue;
		}

		//Line is a comment ignore
		if (line[0] == '#') continue;

		//Parsing
		if (isblank(line[0]))
		{
			//Add command to rules
			Rule* rule = &rules[rules_size-1];
			Command command = {0};
			command.command = strdup(strstrip(line));
			rule->commands[rule->commands_size++] = command;
		}
		else if (isalnum(line[0]))
		{
			char* save = NULL;
			char* token = strtok_r(line, ":", &save);

			Rule rule = {0};
			rule.rule_name = strdup(token);

			while (token != NULL)
			{
				token = strtok_r(NULL, "\n\t ", &save);
				if (token != NULL) rule.files[rule.files_size++] = strdup(token);
			}
			rules[rules_size++] = rule;
		}
	}
	free(line);
	free(line_raw);

	return 0;
}

int execute()
{
	return 0;
}

void debug_stage1()
{
	for (size_t i = 0; i < rules_size; i++)
	{
		Rule rule = rules[i];
		printf("Rule %lu: \n", i+1);
		printf("\tTargets:      %s\n", rule.rule_name);		

		char* files = strjoin(rule.files, rule.files_size, " ");
		printf("\tDependencies: %s\n", files);
		free(files);

		if (rule.commands_size > 0)
		{
			printf("Commands:\n");
			for (size_t ii = 0; ii < rule.commands_size; ii++) printf("\t%s\n", rule.commands[ii].command);
		}
		printf("\n");
	}
}

void free_rules()
{
	for (size_t i = 0; i < rules_size; i++)
	{
		Rule* rule = &rules[i];
		free(rule->rule_name);
		for (size_t ii = 0; ii < rule->files_size; ii++) free(rule->files[ii]);
		for (size_t ii = 0; ii < rule->commands_size; ii++) 
		{
			//Command command = rule->commands[ii];
			//free(command.command);
			//free((&rule->commands[ii])->command);
		}
	}
}

