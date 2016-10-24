/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 25-10-2016
 * Last Modified: Tue 25 Oct 2016 01:48:44 AEDT
 */

#include <stdlib.h>
#include <stdio.h>

#include "hashtable.h"

int main() 
{
	Hashtable* hashtable = malloc(sizeof(Hashtable));
	init_hashtable(hashtable, 1024, sizeof(int));

	Entry* entry = malloc(sizeof(Entry));
	entry->key = "hello";
	entry->data = 15;

	Entry* entry2 = malloc(sizeof(Entry));
	entry2->key = "hello2";
	entry2->data = 84;

	Entry* entry3 = malloc(sizeof(Entry));
	entry3->key = "hello8";
	entry3->data = 69;

	push_hashtable(hashtable, entry);
	push_hashtable(hashtable, entry2);
	push_hashtable(hashtable, entry3);
	
	entry2->data = 48;
	push_hashtable(hashtable, entry2);

	printf("%d\n", get_hashtable(hashtable, "hello")->data);
	printf("%d\n", get_hashtable(hashtable, "hello2")->data);
	printf("%d\n", get_hashtable(hashtable, "hello8")->data);
}
