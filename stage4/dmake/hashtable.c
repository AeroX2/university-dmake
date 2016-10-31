/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 10-08-2016
 * Last Modified: Mon 31 Oct 2016 09:34:25 PM AEDT
 */
#include "hashtable.h"

bool init_hashtable(Hashtable* hashtable, size_t allocated_size, size_t element_size)
{
	if (init_array(&hashtable->buckets, sizeof(Array))) return true;

	size_t i;
	for (i = 0; i < allocated_size; i++)
	{
		Array* array = malloc(sizeof(Array));
		if (array == NULL) return true;
		if (init_array(array, element_size)) return true;
		push_array(&hashtable->buckets, array);
	}
	hashtable->allocated_size = allocated_size;

	return false;
}

bool push_hashtable(Hashtable* hashtable, Entry* entry)
{
	size_t hashed_key = hash(entry->key) % hashtable->allocated_size;
	
	Array* array = (Array*)hashtable->buckets.data[hashed_key];
	if (array->size == 0) push_array(array, entry);
	else
	{
		bool found = false;
		size_t i;
		for (i = 0; i < array->size; i++)
		{
			Entry* entry2 = (Entry*)array->data[i];
			if (strcmp(entry->key, entry2->key) == 0)
			{
				found = true;
				array->data[i] = entry;
				break;
			}
		}

		if (!found) push_array(array, entry);
	}

	return false;
}

void* get_hashtable(Hashtable* hashtable, char* key)
{
	size_t hashed_key = hash(key) % hashtable->allocated_size;
	
	Array* array = (Array*)hashtable->buckets.data[hashed_key];
	if (array->size == 0) return (Entry*)array->data[0];
	else
	{
		size_t i;
		for (i = 0; i < array->size; i++)
		{
			Entry* entry = (Entry*)array->data[i];
			if (strcmp(key, entry->key) == 0)
			{
				return entry->data;
			}
		}
	}

	return NULL;
}

bool exists_hashtable(Hashtable* hashtable, char* key)
{
	size_t hashed_key = hash(key) % hashtable->allocated_size;
	
	Array* array = (Array*)hashtable->buckets.data[hashed_key];
	return array->size > 0;
}


void free_hashtable(Hashtable* hashtable)
{
	size_t i;
	for (i = 0; i < hashtable->buckets.size; i++)
	{
		free_array((Array*)&hashtable->buckets.data[i]);
		hashtable->buckets.data[i] = NULL;
	}
	free_array(&hashtable->buckets);
	//hashtable->buckets = NULL;
}

