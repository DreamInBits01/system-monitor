#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <ctype.h>
#include <ncurses.h>

int is_numeric(char *name);
void build_loadbar(int fill, int bar_width, int y, int x);
#endif
