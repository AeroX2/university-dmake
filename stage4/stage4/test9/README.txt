This is a more complicate Dmake file with several rules.  
oldest is echo.h then echo.c, echo.o, and echo is most recent
For cat program, cat.o is oldest, then cat, then .@cat.o, then .@cat, then cat.c has been modified.
The clean: rule removes echo.o which means that the other rules must rebuild it.
Expected action: remove echo.o, compile echo.o, cannot compare it because there
was no file to save, recompile echo, create memo file .@echo
For cat program: recompile cat.c, discover it has changed, remove memo, recompile cat,
discover it has also changed, remove memo.
