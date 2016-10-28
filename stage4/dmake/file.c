/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 25-10-2016
 * Last Modified: Tue 25 Oct 2016 21:49:09 AEDT
 */

#include "file.h"

bool read_dmake_file()
{
	init_hashtable(&file_times, 1024, sizeof(File));
	FILE* file = NULL; //fopen(".dmake", "rb");
	if (file == NULL) return false;

	char* line = NULL;
	size_t len = 0;
	while (getline(&line, &len, file) != -1)
	{
		Entry* entry = malloc(sizeof(Entry));
		char* token = strtok(line, ",");
		entry->key = token;

		token = strtok(NULL, ",");
		File* file = malloc(sizeof(File));

		char* temp;
		size_t val = strtol(token, &temp, 0);
		if (*temp == '\0') return true;

		file->old_time = val;
		entry->data = file;
	}
	fclose(file);

	return false;
}

bool write_dmake_file()
{
	FILE* f = fopen(".dmake", "wb");

	size_t i;
	size_t ii;
	for (i = 0; i < file_times.buckets.size; i++)
	{
		Array* array = (Array*)file_times.buckets.data[i];
		for (ii = 0; ii < array->size; ii++)
		{
			Entry entry = *(Entry*)array->data[ii];
			File file = *(File*)entry.data;
			fprintf(f, "%s,%lu\n", entry.key, file.new_time);
		}
	}
	fclose(f);
	return false;
}
