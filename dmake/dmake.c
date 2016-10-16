/* Author: James Ridey 44805632
 *         james.ridey@students.mq.edu.au  
 * Creation Date: 13-10-2016
 * Last Modified: Sun 16 Oct 2016 20:36:28 AEDT
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "parser.h"

void usage();

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

	//Parse had an error
	if (output > 0) exit(1);

	if (debug == 0) output = execute();
	else
	{
		switch (debug)
		{
			case 1:
				debug_stage1();
				break;
		//	case 2:
		//		output = debug_stage1();
		//		break;
		}
	}

	free_rules();
	if (output > 0) exit(1);

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
