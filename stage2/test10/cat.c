/* 
A program that has functionality similar to cat.
When no input files are specified, or when input file is -, reads stdin.
Named files are read.
All are written to stdout.
Options:
-o specify output file name instead of stdout.
*/

# include "cat.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

int main(int argc, char **argv) {
    int opt, i, c;
    FILE *infile, *outfile;
    outfile = stdout;
    infile = stdin;
    // Parse options using getopt.
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                outfile = fopen(optarg, "w");
                if (outfile == NULL) {
                    fprintf (stderr, "Could not open %s to write\n", optarg);
                    exit (OPEN_FAILURE);
                }
                break;
        }
    }
    // Check for no input files provided
    if (optind >= argc) {
        // Pretend that the user provided input file name "-"
        argv[argc-1] = "-";
        optind = argc-1;
    }
    // Process all the named files
    for (i = optind; i < argc; i++) {
        if (strcmp (argv[i], "-") == 0) {
            infile = stdin;
        } else {
            infile = fopen(argv[i], "r");
            if (infile == NULL) {
            }
        }
        while ((c = getc(infile)) != EOF) {
            putc(c, stdout);
        }
        fclose (infile);
    }
    fclose (stdout);
}
