#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>

#include "array.h"

typedef struct Hashtable
{
	Array buckets;
	size_t allocated_size;
} Hashtable;

typedef struct Entry
{
	char* key;
	void* data;
} Entry;

bool init_hashtable(Hashtable* hashtable, size_t allocated_size, size_t element_size);
size_t hash(char* string);
bool push_hashtable(Hashtable* hashtable, Entry* entry);
Entry* get_hashtable(Hashtable* hashtable, char* key);
void free_hashtable(Hashtable* hashtable);

#endif
