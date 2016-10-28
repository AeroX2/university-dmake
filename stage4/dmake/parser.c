/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Fri 28 Oct 2016 11:20:14 PM AEDT
 */

#include "parser.h"

Array rules = {};
Array rules_to_fire = {};
Array created_files = {};
Hashtable file_times = {};

int parse(FILE* file)
{
	char* line = NULL;
	char* line_raw = NULL;
	size_t len = 0;
	size_t line_num = 0;

	bool append = false;
	bool in_rule = false;

	init_array(&rules, sizeof(Rule));

	while (getline(&line_raw, &len, file) != -1)
	{
		line_num++;
		size_t length = line != NULL ? strlen(line) : 0;
		size_t length_raw = strlen(line_raw);

		if (append) 
		{
			line[length-2] = '\0';
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

		//TODO This detects backslashes with spaces at the end but line 50 needs to be fixed before you can use this
		/*bool stop = false;
		for (i = length+1; i-- > 0;)
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
		if (line[length-2] == '\\')
		{
			append = true;
			continue;
		}

		//Line is a comment ignore
		bool ignore = false;
		size_t i;
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
			Command* command = malloc(sizeof(Command));

			command->command = strdup(strstrip(line, "\t\n "));
			push_array(&rule->commands, command);
		}
		else if (isalnum(line[0]))
		{
			char* save = NULL;
			char* token = strtok_r(line, ":", &save);

			Rule* rule = malloc(sizeof(Rule));

			init_array(&rule->dependencies, sizeof(char*));
			init_array(&rule->commands, sizeof(Command));

			if (strlen(token) != length) rule->target = strdup(strstrip(token, "\t\n "));
			else
			{
				fprintf(stderr, "Syntax error at line %lu: Expected colon separator in rule header:\n", line_num);
				fprintf(stderr, "%s", line);
				return SYNTAX_RULE;
			}

			while (token != NULL)
			{
				token = strtok_r(NULL, "\n\t ", &save);
				if (token != NULL) push_array(&rule->dependencies, strdup(token));	
			}
			push_array(&rules, rule);

			in_rule = true;
		}

		if (terminate) return INTERRUPT;
	}
	free(line);
	free(line_raw);

	return SUCCESS;
}

int order()
{
	init_array(&rules_to_fire, sizeof(size_t));
	init_array(&created_files, sizeof(char*));

	size_t i;
	size_t ii;
	size_t iii;
	for (i = rules.size; i-- > 0;)
	{
		Rule rule = *(Rule*)rules.data[i];	

		struct stat target_stat;
		bool target_exists = stat(rule.target, &target_stat) >= 0;

		time_t target_time;
		//Entry* entry = get_hashtable(&file_times, rule.target);
		void* entry = NULL;
		if (entry == NULL) target_time = target_stat.st_mtime;
		//else target_time = (time_t)entry->data;

		bool fire = rule.dependencies.size == 0;
		for (ii = 0; ii < rule.dependencies.size; ii++)
		{
			char* dependency = rule.dependencies.data[ii];
			//printf("Depend %s\n",dependency);
			struct stat dependency_stat;

			bool found = false;
			for (iii = 0; iii < created_files.size; iii++)
			{
				if (strcmp(created_files.data[iii], dependency) == 0) found = true;
			}
			if (found) fire = true;
			else if (stat(dependency, &dependency_stat) == 0)
			{
				//printf("Target %s\n", rule.rule_name);
				if (!target_exists) fire = true;
				else if (dependency_stat.st_mtime > target_time) fire = true;
			}
			else
			{
				fprintf(stderr, "Error: No rule to build %s\n", dependency);
				return NO_RULE_TO_FIRE;
			}
		}
			
		if (fire)
		{
			push_array(&rules_to_fire, (void*)i);
			push_array(&created_files, rule.target);

			/*Entry* entry = malloc(sizeof(Entry));
			entry->key = rule.target;

			size_t hash = filehash(rule.target);
			File* file = malloc(sizeof(File));
			file->hash = hash;
			file->old_time = target_time;
			entry->data = file;
			push_hashtable(&file_times, entry);*/
		}

		if (terminate) return INTERRUPT;
	}
	return SUCCESS;
}

int execute()
{
	size_t i;
	size_t ii;
	int iii;
	for (i = 0; i < rules_to_fire.size; i++)
	{
		Rule rule = *(Rule*)rules.data[(size_t)rules_to_fire.data[i]];

		for (ii = 0; ii < rule.commands.size; ii++)
		{
			Command command = *(Command*)rule.commands.data[ii];
			char modifiers = 0;

			for (iii = 0; command.command[iii] != '\0'; iii++)
			{
				bool stop = false;
				switch (command.command[iii])
				{
					case '@':
						modifiers |= AT_MODIFIER;
						break;
					case '-':
						modifiers |= DASH_MODIFIER;
						break;
					case '=':
						modifiers |= EQUALS_MODIFIER;
						break;
					case '*':
						modifiers |= STAR_MODIFIER;
						break;
					case '&':
						modifiers |= AMPERSAND_MODIFIER;
						break;
					case ' ':
						break;
					default:
						stop = true;
				}
				if (stop) break;
			}

			pid_t pid = fork();
			if (pid == -1) return FAILURE_FORK;
			else if (pid <= 0)
			{
				//Child
				//signal(SIGINT, child_signal_handler);

				int fd = open("/dev/null", O_WRONLY);
				if (modifiers & STAR_MODIFIER) dup2(fd, 1);
				if (modifiers & AMPERSAND_MODIFIER) dup2(fd, 2);
				close(fd);
				
				execl("/bin/sh","/bin/sh","-c",command.command + iii,NULL);
				_exit(0);
			}
			else
			{
				//Parent
				if (!(modifiers & AT_MODIFIER)) 
				{
					printf("%s\n", command.command + iii);
					fflush(NULL);
				}

				int status;
				waitpid(pid, &status, 0);
				if (status != 0)
				{
					if (!(modifiers & EQUALS_MODIFIER)) 
					{
						if (modifiers & DASH_MODIFIER) fprintf(stderr, "--- Exited with error 1 (ignored) ---\n");
						else fprintf(stderr, "*** Exited with error 1 ***\n");
					}
					if (!(modifiers & DASH_MODIFIER) && !(modifiers & EQUALS_MODIFIER)) return status;	
				}

				if (terminate) return INTERRUPT;
			}
		}

		/*Entry* entry = get_hashtable(&file_times, rule.target);
		if (entry != NULL)
		{
			File* file = (File*)entry->data;
			if (filehash(rule.target) != file->hash)
			{
				struct utimbuf new_time;
				new_time.modtime = file->old_time;
				utime(rule.target, &new_time);
				file->new_time = time(NULL);
			}
		}*/
	}

	return SUCCESS;
}

void debug_stage1()
{
	size_t i;
	for (i = 0; i < rules.size; i++)
	{
		Rule rule = *(Rule*)rules.data[i];
		printf("Rule %lu:\n", i+1);
		printf("    Targets:      %s\n", rule.target);

		printf("    Dependencies: ");
		if (rule.dependencies.size > 0)
		{
			char* files = strjoin((char**)rule.dependencies.data, rule.dependencies.size, ", ");
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

void debug_stage2()
{
	size_t i;
	for (i = 0; i < rules_to_fire.size; i++)
	{
		printf("Fire rule %lu\n", (size_t)rules_to_fire.data[i]+1);
	}
}

void free_rules()
{
	size_t i;
	size_t ii;
	for (i = 0; i < rules.size; i++)
	{
		Rule* rule = (Rule*)rules.data[i];
		free(rule->target);
		free_array(&rule->dependencies);
		for (ii = 0; ii < rule->commands.size; ii++) 
		{
			Command* command = rule->commands.data[ii];
			free(command->command);
		}
		free_array(&rule->commands);
	}

	free_array(&rules);
	free_array_fun(&rules_to_fire, NULL);
	free_array_fun(&created_files, NULL);
}

void child_signal_handler()
{
	printf("Child interrupt detected\n");
	terminate = true;
}
