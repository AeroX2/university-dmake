/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Thu 13 Oct 2016 16:06:27 AEDT
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

	char* files[100];
	size_t files_size;

	Command commands[1000];
	size_t commands_size;
} Rule;

Rule rules[1000];
size_t rules_size = 0;

int parse(FILE* file)
{
	char* line = NULL;
	char* previous_line = NULL;
	size_t len = 0;

	int append = 0;
	int inside_rule = 0;

	while (getline(&line, &len, file) != -1)
	{
		//Line is a comment ignore
		for (size_t i = 0; i < strlen(line) && isblank(line[i]); i++) if (line[i] == '#') continue;
		if (is_blankline(line, strlen(line))) inside_rule = 0;
		if (append) line = strcat(line, previous_line);

		//Parsing
		if (inside_rule)
		{
			//Add command to rules
			Rule* rule = &rules[rules_size-1];
			Command command = {0};
			command.command = strdup(strstrip(line));
			rule->commands[rule->commands_size++] = command;
		}
		else
		{
			char* save = NULL;
			char* token = strtok_r(line, "\n\t ", &save);

			Rule rule = {0};
			rule.rule_name = strdup(token);

			while (token != NULL)
			{
				token = strtok_r(NULL, "\n\t ", &save);
				//TODO Is there a better way?
				if (token != NULL) rule.files[rule.files_size++] = strdup(token);
			}
			inside_rule = 1;
			rules[rules_size++] = rule;
		}

		//Check if line has a backslash
		while (*line++ != '\0')
		{
			if (isblank(*line)) append = 0;
			else if (*line == '\\') append = 1;
		}
		if (append) previous_line = strdup(line);
	}
	free(previous_line);
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
		printf("Rule %lu: \n", i);
		printf("\tTargets:      %s\n", rule.rule_name);		

		char* files = strjoin(rule.files, rule.files_size, " ");
		printf("\tDependencies: %s\n", files);
		free(files);

		if (rule.commands_size > 0)
		{
			printf("Commands:\n");
			for (size_t ii = 0; ii < rule.commands_size-1; ii++) printf("\t%s\n", rule.commands[ii].command);
		}
	}
}

