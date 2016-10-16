/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Mon 17 Oct 2016 02:07:56 AEDT
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
	size_t line_num = 0;

	bool append = false;
	bool in_rule = false;

	while (getline(&line_raw, &len, file) != -1)
	{
		line_num++;
		size_t length = line != NULL ? strlen(line) : 0;
		size_t length_raw = strlen(line_raw);

		size_t i;

		if (append) 
		{
			line[length-2] = '\0';

			//I only want to strip the beginning and only if the line contains characters
			/*size_t start = 0;
			if (strfind(line_raw, length_raw, isalnum)) 
			{
				for (i = 0; i < length_raw; i++)
				{
					if (!isblank(line_raw[i]))
					{
						start = i;
						break;
					}
				}
			}*/

			line = realloc(line, length+length_raw-1);
			strcat(line, line_raw);

			append = false;
		}
		else 
		{
			if (length_raw > length) line = realloc(line, length_raw+1);
			strcpy(line, line_raw);
		}

		//TODO strlen is expensive replace with length and length_raw
		length = strlen(line);

		bool stop = false;
		for (i = length-1; i > 0; i--)
		{
			if (line[i] == '\\')
			{
				stop = true;
				break;
			}
			else if (isalnum(line[i])) break;
		}
		//Line has a back slash append line
		/*if (line[length-2] == '\\')
		{
			append = true;
			continue;
		}*/
		if (stop) 
		{
			append = true;
			continue;
		}

		//Line is a comment ignore
		bool ignore = false;
		for (i = 0; i < length; i++)
		{
			if (line[i] == '#') 
			{
				ignore = true;
				break;
			}
			else if (!isblank(line[i])) break;
		}
		if (ignore) continue;

		//Parsing
		if (isblank(line[0]))
		{
			if (!in_rule)
			{
				fprintf(stderr, "Syntax error at line %lu: Unexpected command without rule header:\n", line_num);
				fprintf(stderr, "%s", line);
				return 1;
			}

			if (!strfind(line, length, isalnum)) 
			{
				in_rule = false;
				continue;
			}

			//Add command to rules
			Rule* rule = &rules[rules_size-1];
			Command command = {0};
			command.command = strdup(strstrip(line, "\t\n "));
			rule->commands[rule->commands_size++] = command;
		}
		else if (isalnum(line[0]))
		{
			char* save = NULL;
			char* token = strtok_r(line, ":", &save);

			Rule rule = {0};
			if (strlen(token) != length) rule.rule_name = strdup(strstrip(token, "\t\n "));
			else
			{
				fprintf(stderr, "Syntax error at line %lu: Expected colon separator in rule header:\n", line_num);
				fprintf(stderr, "%s", line);
				return 1;
			}

			while (token != NULL)
			{
				token = strtok_r(NULL, "\n\t ", &save);
				if (token != NULL) rule.files[rule.files_size++] = strdup(token);
			}
			rules[rules_size++] = rule;
			in_rule = true;
		}
	}
	free(line);
	//free(line_raw);

	return 0;
}

int execute()
{
	return 0;
}

void debug_stage1()
{
	size_t i;
	for (i = 0; i < rules_size; i++)
	{
		Rule rule = rules[i];
		printf("Rule %lu:\n", i+1);
		printf("    Targets:      %s\n", rule.rule_name);

		printf("    Dependencies: ");
		if (rule.files_size > 0)
		{
			char* files = strjoin(rule.files, rule.files_size, ", ");
			printf("%s", files);
			free(files);
		}
		printf("\n");

		if (rule.commands_size > 0)
		{
			printf("Commands:\n");
			size_t ii;
			for (ii = 0; ii < rule.commands_size; ii++) printf("    %s\n", rule.commands[ii].command);
		}
		printf("\n");
	}
}

void free_rules()
{
	size_t i;
	size_t ii;
	for (i = 0; i < rules_size; i++)
	{
		Rule* rule = &rules[i];
		free(rule->rule_name);
		for (ii = 0; ii < rule->files_size; ii++) free(rule->files[ii]);
		for (ii = 0; ii < rule->commands_size; ii++) 
		{
			//Command command = rule->commands[ii];
			//free(command.command);
			//free((&rule->commands[ii])->command);
		}
	}
}

