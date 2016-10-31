This is a more complicate Dmake file with several rules. 
oldest is usage.c, then echo.h, usage.o, echo.o, echo, .@echo.o and echo.c is most recent
For cat program, cat.o is oldest, then cat, then cat.c which has been changed.
Expected action: compile echo.o, discover it has not changed, replace the memo file,
Because say does not exist, run the rule to recompile echo and say, discover that
   echo is unchanged and create a memo for it.
For cat program, recompile cat.o and discover it has changed, recompile cat.
