This is a Dmakefile with four rules to build a simple program and a variant called say.
There is a common dependency on echo.o.
Test case where echo.c is newest, echo.o is missing and echo is missing.
The program say also does not exist.
The required file say.o does not exist.
There is a clean up rule to remove echo.o which uses modifiers & and =
