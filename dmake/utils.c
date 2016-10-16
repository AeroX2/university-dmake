/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Sun 16 Oct 2016 02:08:57 PM AEDT
 */

#include "utils.h"

char* strstrip(char* string)
{
	int start = 0;
	int end = 0;

	size_t length = strlen(string);
	size_t i;
	for (i = 0; i < length; i++)
	{
		if (!isblank(string[i])) break;
		start++;
	}

	for (i = length-1; i > 0; i--)
	{
		end = i-1;
		if (string[i] == '\n') break;
	}

	string += start;
	string[end] = '\0';

	return string;
}

//The string created by strjoin must be freed
char* strjoin(char** strings, size_t strings_len, char* delimiter)
{
	if (strings_len <= 0) 
	{
		printf("strjoin has nothing to join\n");
		return "error";
	}

	size_t length = 0;
	size_t i;
	for (i = 0; i < strings_len; i++) length += strlen(strings[i])+1;
	char* string = malloc(length-1);

	strcpy(string, strings[0]);
	for (i = 1; i < strings_len; i++) 
	{
		strcat(string, delimiter);
		strcat(string, strings[i]);	
	}
	return string;
}
