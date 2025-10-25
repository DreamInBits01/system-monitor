#ifndef MEMORY_H
#define MEMORY_H
#define KB_TO_GB(x) ((x) / (1024 * 1024))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "structs.h"
#include "helpers.h"

void read_memory_info(MemoryInfo *memory_info);
void show_memory_info(MemoryInfo *memory_info, int bar_width);
void build_loadbar(int fill, int bar_width, int y, int x);
#endif