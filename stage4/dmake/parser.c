/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Thu 03 Nov 2016 01:49:11 AM AEDT
 */

#include "parser.h"

Array rules = {};
Array rules_to_fire = {};
Array created_files = {};
//Hashtable target_dont_fire = {};

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

			init_array(&rule->targets, sizeof(char*));
			init_array(&rule->dependencies, sizeof(char*));
			init_array(&rule->commands, sizeof(Command));

			if (strlen(token) != length) 
			{
				char* save2 = NULL;
				char* token2 = strtok_r(token, " ", &save2);
				while (token2 != NULL)
				{
					push_array(&rule->targets, strdup(strstrip(token2, "\t\n ")));	
					token2 = strtok_r(NULL, "\n\t ", &save2);
				}
			}
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
	size_t j;
	size_t ii;
	size_t iii;
	for (i = rules.size; i-- > 0;)
	{
		Rule rule = *(Rule*)rules.data[i];	

		struct stat target_stat;
		target_stat.st_mtime = 0;

		time_t target_time = 0;
		bool target_time_set = false;
		bool target_exists = false;
		for (j = 0; j < rule.targets.size; j++) 
		{
			char* target = rule.targets.data[j];
			if (stat(target, &target_stat) == 0)
			{
				if (!target_time_set) 
				{
					target_time = target_stat.st_mtime;
					target_time_set = true;
				}
				target_time = min(target_time, target_stat.st_mtime);
				target_exists = true;
			}
		}

		bool fire = rule.dependencies.size == 0;
		for (ii = 0; ii < rule.dependencies.size; ii++)
		{
			char* dependency = rule.dependencies.data[ii];
			struct stat dependency_stat;

			bool found = false;
			for (iii = 0; iii < created_files.size; iii++)
			{
				if (strcmp(created_files.data[iii], dependency) == 0) found = true;
			}
			if (found) fire = true;
			else if (stat(dependency, &dependency_stat) == 0)
			{
				if (!target_exists) fire = true;
				else if (dependency_stat.st_mtime >= target_time) fire = true;
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

			for (j = 0; j < rule.targets.size; j++) 
			{
				char* target = rule.targets.data[j];
				push_array(&created_files, target);
			}
		}
	}
	return SUCCESS;
}

int execute(bool debug)
{
	//init_hashtable(&target_dont_fire,1024,sizeof(char*));

	size_t i;
	size_t j;
	size_t ii;
	for (i = rules.size; i-- > 0;)
	{
		Rule rule = *(Rule*)rules.data[i];
		//printf("Target start %s\n", strjoin((char**) rule.targets.data, rule.targets.size, ", "));

		bool target_exists = false;

		bool ttime_set = false;
		time_t ttime = 0;

		char* backup_file[rule.targets.size];
		char* timestamp_file[rule.targets.size];
		for (j = 0; j < rule.targets.size; j++) 
		{
			char* target = rule.targets.data[j];
			size_t target_length = strlen(target)+3;

			backup_file[j] = malloc(target_length);
			strcpy(backup_file[j], ".~");
			strcat(backup_file[j], target);	

			timestamp_file[j] = malloc(target_length);
			strcpy(timestamp_file[j], ".@");	
			strcat(timestamp_file[j], target);	

			struct stat timestamp_stat;
			struct stat target_stat;
			timestamp_stat.st_mtime = 0;
			target_stat.st_mtime = 0;

			stat(timestamp_file[j], &timestamp_stat);
			if (stat(target, &target_stat) == 0)
			{
				target_exists = true;
				if (!ttime_set)
				{
					ttime = max(target_stat.st_mtime, timestamp_stat.st_mtime);
					ttime_set = true;
				}
				ttime = min(ttime, max(target_stat.st_mtime, timestamp_stat.st_mtime));
			}
			else ttime = 1;

			if (timestamp_stat.st_mtime < target_stat.st_mtime) unlink(timestamp_file[j]);

			//printf("%d %s\n", i, target);
		}

		if (ttime == 0)
		{
			//printf("Snakes\n");
			ttime = time(NULL);
		}

		bool in_to_fire = in_array(&rules_to_fire, (void*)i);


		//if (in_to_fire) printf("Test 1\n");

		//Check if one of the dependencies is different
		bool fire = false;
		for (j = 0; j < rule.dependencies.size; j++)
		{
			struct stat d;
			char* dependency = rule.dependencies.data[j];
			//printf("%s\n", dependency);

			time_t dtime = stat(dependency, &d) == 0 ? d.st_mtime : time(NULL);

			if (dtime >= ttime) 
			{
				//printf("%lu\n", dtime);
				//printf("%lu\n", ttime);
				fire = true;
				break;
			}
		}

		//if (fire) printf("Test 2\n");

		if (!target_exists) fire = true;
		//if (fire) printf("Test 3\n");

		if (in_to_fire && rule.dependencies.size == 0) fire = true;
		//if (fire) printf("Test 4\n");

		if (fire) 
		{

			//Make backup copy of file
			for (j = 0; j < rule.targets.size; j++) 
			{
				char* target = rule.targets.data[j];
				rename(target, backup_file[j]);
			}

			for (ii = 0; ii < rule.commands.size; ii++)
			{
				Command command = *(Command*)rule.commands.data[ii];

				char* times[3] = {NULL};
				int modifiers = 0;
				int offset = 0;
				execute_modifiers(command.command, &modifiers, times, &offset);
				int error = execute_command(command.command, modifiers, times, offset);
				if (error != 0) return error;
			}
		}

		for (j = 0; j < rule.targets.size; j++) 
		{
			char* target = rule.targets.data[j];
			//If the file is the same
			if (access(backup_file[j], F_OK) != -1 && !filecmp(target, backup_file[j]))
			{
				if (debug) printf("Dmake: File %s unchanged.\n", target);
				/*Entry* entry = malloc(sizeof(Entry));
				entry->key = strdup(target);
				entry->data = NULL;
				push_hashtable(&target_dont_fire, entry);*/

				rename(backup_file[j], target);
				int handle = creat(timestamp_file[j], S_IRUSR | S_IRGRP | S_IROTH);
				close(handle);
			}
			else 
			{
				unlink(backup_file[j]);
				if (fire) unlink(timestamp_file[j]);
			}

			free(backup_file[j]);
			free(timestamp_file[j]);
		}
	}

	return SUCCESS;
}

void execute_modifiers(char* command, int* modifiers, char** times, int* offset)
{
	size_t index = 2;
	bool open_bracket = false;

	size_t i;
	for (i = 0; command[i] != '\0'; i++)
	{
		bool stop = false;
		char c = command[i];
		switch (c)
		{
			case '@':
				*modifiers |= AT_MODIFIER;
				break;
			case '-':
				*modifiers |= DASH_MODIFIER;
				break;
			case '=':
				*modifiers |= EQUALS_MODIFIER;
				break;
			case '*':
				*modifiers |= STAR_MODIFIER;
				break;
			case '&':
				*modifiers |= AMPERSAND_MODIFIER;
				break;
			case '[':
				open_bracket = true;
				break;
			case ':':
				if (!open_bracket) stop = true;
				index--;
				*modifiers |= SECONDS_MODIFIER;
				break;
			case '.':
				if (!open_bracket) stop = true; 
				index--;
				break;
			case ']':
				open_bracket = false;
				break;
			case ' ':
				break;
			default:
				if (!isdigit(c)) 
				{
					stop = true;
					break;
				}

				size_t length = times[index] == NULL ? 0 : strlen(times[index]);
				if (length == 0) times[index] = calloc(2,sizeof(char));
				else times[index] = realloc(times[index], length+2);

				times[index] = strncat(times[index], &c, 1);
				times[index][length+1] = '\0';
		}
		if (stop) break;
	}
	*offset = i;
}

int execute_command(char* command, int modifiers, char** times, int offset)
{
	struct timeval timeout = {0,0};
	int i;
	int j = 1;
	for (i = 0; i < 3; i++)
	{
		if (times[i] == NULL) continue;

		int time = atoi(times[i]);
		if (time == 0) return NUMBER_FORMAT;

		if ((modifiers & SECONDS_MODIFIER) && i == 1) timeout.tv_usec = time * 10000;
		else
		{
			timeout.tv_sec += time * j;
			j *= 60;
		}
	}

	for (i = 0; i < 3; i++) free(times[i]);

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
		
		size_t x = count_digits((long)timeout.tv_sec);
		size_t y = count_digits(timeout.tv_usec/10000);
		char* time = malloc(1+x+y);
		sprintf(time, "%llu.%lu", (long long)timeout.tv_sec, timeout.tv_usec);

		struct itimerval timerval = {};
		timerval.it_interval.tv_sec = 0;
		timerval.it_interval.tv_usec = 0;
		timerval.it_value = timeout;
		setitimer(ITIMER_REAL,&timerval,NULL);
		execlp("/bin/sh","/bin/sh","-c",command + offset,NULL);

		free(time);
		_exit(0);
	}
	else
	{
		//Parent
		if (!(modifiers & AT_MODIFIER)) 
		{
			printf("%s\n", command + offset);
			fflush(NULL);
		}

		int status;
		waitpid(pid, &status, 0);
		if (status != 0 && WTERMSIG(status) != SIGALRM)
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
	return SUCCESS;
}

void debug_stage1()
{
	size_t i;
	for (i = 0; i < rules.size; i++)
	{
		Rule rule = *(Rule*)rules.data[i];
		printf("Rule %lu:\n", i+1);
		printf("    Targets:      ");
		if (rule.targets.size > 0)
		{
			char* files = strjoin((char**)rule.targets.data, rule.targets.size, ", ");
			printf("%s", files);
			free(files);
		}
		printf("\n");

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
		free_array(&rule->targets);
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

