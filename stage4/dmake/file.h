#ifndef FILE_H
#define FILE_H

#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "hashtable.h"

extern Hashtable file_times;

bool read_dmake_file();
bool write_dmake_file();

#endif
