/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 10-08-2016
 * Last Modified: Tue 01 Nov 2016 06:57:43 PM AEDT
 */
#include "array.h"

bool init_array(Array* array, size_t element_size)
{
	array->element_size = element_size;
	void** tmp = malloc(array->element_size * sizeof(void*));
	if (tmp) array->data = tmp;
	else 
	{
		free(tmp);
		return true;
	}

	array->allocated_size = 1;
	array->size = 0;
	return false;
}

bool check_array(Array* array)
{
	if (array->size >= array->allocated_size)
	{
		array->allocated_size *= 2;
		void** tmp = realloc(array->data, array->allocated_size * array->element_size * sizeof(void*));
		if (tmp) array->data = tmp;
		else 
		{
			free(tmp);
			return true;
		}
	}
	return false;
}

bool in_array(Array* array, void* data)
{
	size_t i;
	for (i = 0; i < array->size; i++)
	{
		if (array->data[i] == data) return true;
	}
	return false;
}

bool push_array(Array* array, void* data)
{
	array->data[array->size++] = data;
	return check_array(array);
}

void free_array_fun(Array* array, void (*f)(void*))
{
	size_t i;
	for (i = 0; i < array->size; i++)
	{
		if (f != NULL) f(array->data[i]);
		array->data[i] = NULL;
	}
	free(array->data);
	array->data = NULL;
}

void free_array(Array* array)
{
	free_array_fun(array, free);
}
