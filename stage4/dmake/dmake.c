/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Mon 24 Oct 2016 04:28:23 PM AEDT
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "parser.h"

void usage();
void finish(int code);

int main(int argc, char **argv) 
{
	char* filename = "Dmakefile";
	int opt = -1;
	int debug = 0;

    // Parse the command line using getopt.
    while ((opt = getopt(argc, argv, "hf:z:")) >= 0) 
	{
        switch (opt) 
		{
            case 'h':
                // Help
                usage();
                exit(0);
			case 'f':
				filename = optarg;
				break;
			case 'z':
				debug = atoi(optarg);
				if (debug == 0)
				{
					usage();
					exit(1);
				}
				break;
            case '?':
                exit(1);
        }
    }

	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Could not read Dmakefile\n");
		exit(1);
	}
	int output = parse(file);
	fclose(file);

	//Parsing the file had an error
	if (output > 0) finish(output);
	if (debug == 1) 
	{
		debug_stage1();
		finish(0);
	}

	output = order();

	//Determining the order of execution failed
	if (output > 0) finish(output);
	if (debug == 2) 
	{
		debug_stage2();
		finish(0);
	}

	output = execute();
	finish(output);

    return 0;
}

void usage() 
{
    printf("Usage: dmake [-h] [-zN] [-f file]\n");
    printf("  -h: Print this help information.\n");
    printf("  -zN: Perform processing for stage N.\n");
    printf("  -f file: Read dmake rules from file.\n");
    return;
}

void finish(int code)
{
	free_rules();
	exit(code);
}
