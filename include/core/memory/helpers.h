#ifndef MEMORY_HELPERS
#define MEMORY_HELPERS
#define KB_TO_GB(x) ((x) / (1024 * 1024))

#include <stdio.h>
#include <string.h>

#include <ncurses.h>
#include "structs.h"
void parse_memory_line(char *line, void *data);
void build_loadbar(WINDOW *window, int fill, int bar_width, int y, int x);
#endif