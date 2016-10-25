/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 25-10-2016
 * Last Modified: Tue 25 Oct 2016 19:45:47 AEDT
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
	/*FILE* file = fopen(".dmake", "wb");
	//TODO Go through every element in the hashtable and store it in the file
	fclose(file);*/
	return false;
}
