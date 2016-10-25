/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Tue 25 Oct 2016 18:43:22 AEDT
 */

#include "utils.h"

//TODO Use a macro instead
/*
bool safe_malloc(void** pointer, size_t size)
{
	void* tmp = malloc(size);
	if (tmp == NULL)
	{
		fprintf(stderr,"Unable to allocate enough memory with malloc\n");
		return true;
	}
	*pointer = tmp;
	return false;
}

bool safe_realloc(void** pointer, size_t size)
{
	void* tmp = realloc(*pointer,size);
	if (tmp == NULL)
	{
		fprintf(stderr,"Unable to allocate enough memory with realloc\n");
		return true;
	}
	*pointer = tmp;
	return false;
}
*/

char* strstrip(char* string, char* strip)
{
	int length = strlen(string);
	int length_strip = strlen(strip);

	int i;
	int ii;
	int start = 0;
	for (i = 0; i < length; i++)
	{
		bool found = false;
		for (ii = 0; ii < length_strip; ii++)
		{
			if (string[i] == strip[ii]) 
			{
				start = i+1;
				found = true;
				break;
			}
		}
		if (!found) break;
	}

	int end = length;
	for (i = length+1; i-- > 0;)
	{
		bool found = false;
		for (ii = 0; ii < length_strip; ii++)
		{
			if (string[i] == strip[ii]) 
			{
				end = i;
				found = true;
				break;
			}
		}
		if (found) break;
	}

	string[end] = '\0';
	string += start;

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
	for (i = 0; i < strings_len; i++) length += strlen(strings[i])+strlen(delimiter);
	char* string = malloc(length-1);

	strcpy(string, strings[0]);
	for (i = 1; i < strings_len; i++) 
	{
		strcat(string, delimiter);
		strcat(string, strings[i]);	
	}
	return string;
}

bool strfind(char* string, size_t length, int (*f)(int))
{
	size_t i;
	for (i = 0; i < length; i++)
	{
		if (f(string[i])) return true;
	}
	return false;
}

size_t hash(char* string)
{
	/*Borrowed from http://www.partow.net/programming/hashfunctions*/	
	size_t hash = 5381;
	size_t i;
	for (i = 0; string[i] != '\0'; i++)
	{
		hash = ((hash << 5) + hash) + string[i];
	}
	return hash;
}

size_t filehash(char* filename)
{
	/*Borrowed from http://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer*/
	FILE* f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); 

	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = '\0';
	return hash(string);
}
