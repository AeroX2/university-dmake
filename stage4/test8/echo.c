# include <stdio.h>
# include "echo.h"

int main(int argc, char **argv) {
    int i;
    for (i = 1; i < argc; i++)
        PRINT ("%s ", argv[i]);
    PRINT ("\n");
    return 0;
}
