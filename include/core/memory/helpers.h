#ifndef MEMORY_HELPERS
#define MEMORY_HELPERS

#include <stdio.h>
#include <string.h>

#include <ncurses.h>
#include "structs.h"
#include "utils.h"
void parse_memory_line(char *line, void *data);
void build_loadbar(WINDOW *window, int fill, int bar_width, int y, int x);
#endif