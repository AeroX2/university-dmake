echo.c is more recent than echo.o which is more recent than echo.c
memo .@echo.o is more recent than echo.c and memo .@echo is most recent
cat.c is more recent than cat.o which is more recent than cat.  cat.c has been
  changed significantly since cat was built, but not since cat.o was built.
Expected action: No compilation of echo, but cat.o will be recompiled
and found to be the same, then cat will be recompiled and found to be different.
