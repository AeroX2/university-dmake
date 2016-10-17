/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 10-08-2016
 * Last Modified: Mon 17 Oct 2016 22:28:29 AEDT
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

bool push_array(Array* array, void* data)
{
	array->data[array->size++] = data;
	return check_array(array);
}

void free_array(Array* array)
{
	size_t i;
	for (i = 0; i < array->size; i++)
	{
		free(array->data[i]);
		array->data[i] = NULL;
	}
	free(array->data);
	array->data = NULL;
}

