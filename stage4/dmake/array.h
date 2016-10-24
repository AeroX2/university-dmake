#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct Array
{
	void** data;
	size_t size;
	size_t allocated_size;
	size_t element_size;
} Array;

bool init_array(Array* array, size_t element_size);
bool push_array(Array* array, void* data);
bool check_array(Array* array);
void free_array(Array* array);

#endif
