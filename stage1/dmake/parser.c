/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Thu 03 Nov 2016 03:45:42 PM AEDT
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

	//char* files[1000];
	//size_t files_size;
	Array files;
	Array commands;

} Rule;

Array rules = {0};

int parse(FILE* file)
{
	char* line = NULL;
	char* line_raw = NULL;
	size_t len = 0;
	size_t line_num = 0;

	bool append = false;
	bool in_rule = false;

	if (init_array(&rules, sizeof(Rule))) return MALLOC_FAIL;

	while (getline(&line_raw, &len, file) != -1)
	{
		line_num++;
		size_t length = line != NULL ? strlen(line) : 0;
		size_t length_raw = strlen(line_raw);

		size_t i;

		if (append) 
		{
			line[length-2] = '\0';
			if (safe_realloc((void**)&line, length+length_raw-1)) return MALLOC_FAIL;

			strcat(line, line_raw);
			append = false;
		}
		else 
		{
			if (length_raw > length) 
			{
				if (safe_realloc((void**)&line, length_raw+1)) return MALLOC_FAIL;
			}
			strcpy(line, line_raw);
		}

		//TODO strlen is expensive replace with length and length_raw
		length = strlen(line);

		//TODO This detects backslashes with spaces at the end but line 50 needs to be fixed before you can use this
		/*bool stop = false;
		for (i = length-1; i > 0; i--)
		{
			if (line[i] == '\\')
			{
				stop = true;
				break;
			}
			else if (isalnum(line[i])) break;
		}
		if (stop) 
		{
			append = true;
			continue;
		}*/
		if (length >= 2 && line[length-2] == '\\')
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
				return SYNTAX_COLON;
			}

			if (!strfind(line, length, isalnum)) 
			{
				in_rule = false;
				continue;
			}

			//Add command to rules
			Rule* rule = (Rule*)rules.data[rules.size-1];
			Command* command = NULL;
			if (safe_malloc((void**)&command, sizeof(Command))) return MALLOC_FAIL;

			command->command = strdup(strstrip(line, "\t\n "));
			if (push_array(&rule->commands, command)) return MALLOC_FAIL;
		}
		else if (isalnum(line[0]))
		{
			char* save = NULL;
			char* token = strtok_r(line, ":", &save);

			Rule* rule = NULL;
			if (safe_malloc((void**)&rule, sizeof(Rule))) return MALLOC_FAIL;

			if (init_array(&rule->files, sizeof(char*))) return MALLOC_FAIL;
			if (init_array(&rule->commands, sizeof(Command))) return MALLOC_FAIL;

			if (strlen(token) != length) rule->rule_name = strdup(strstrip(token, "\t\n "));
			else
			{
				fprintf(stderr, "Syntax error at line %lu: Expected colon separator in rule header:\n", line_num);
				fprintf(stderr, "%s", line);
				return SYNTAX_RULE;
			}

			while (token != NULL)
			{
				token = strtok_r(NULL, "\n\t ", &save);
				if (token != NULL)
				{ 
					if (push_array(&rule->files, strdup(token))) return MALLOC_FAIL;	
				}
			}
			if (push_array(&rules, rule)) return MALLOC_FAIL;

			in_rule = true;
		}
	}
	free(line);
	free(line_raw);

	return SUCCESS;
}

int execute()
{
	return SUCCESS;
}

void debug_stage1()
{
	size_t i;
	for (i = 0; i < rules.size; i++)
	{
		Rule rule = *(Rule*)rules.data[i];
		printf("Rule %lu:\n", i+1);
		printf("    Targets:      %s\n", rule.rule_name);

		printf("    Dependencies: ");
		if (rule.files.size > 0)
		{
			char* files = strjoin((char**)rule.files.data, rule.files.size, ", ");
			printf("%s", files);
			free(files);
		}
		printf("\n");

		if (rule.commands.size > 0)
		{
			printf("Commands:\n");
			size_t ii;
			for (ii = 0; ii < rule.commands.size; ii++) printf("    %s\n", (*(Command*)rule.commands.data[ii]).command);
		}
		printf("\n");
	}
}

void free_rules()
{
	size_t i;
	size_t ii;
	for (i = 0; i < rules.size; i++)
	{
		Rule* rule = (Rule*)rules.data[i];
		free(rule->rule_name);
		free_array(&rule->files);
		for (ii = 0; ii < rule->commands.size; ii++) 
		{
			Command* command = rule->commands.data[ii];
			free(command->command);
		}
		free_array(&rule->commands);
	}
	free_array(&rules);
}

