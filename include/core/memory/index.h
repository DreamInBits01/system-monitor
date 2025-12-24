#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>
#include <ncurses.h>
#include "structs.h"
#include "helpers.h"
#include "context/structs.h"
#include "utils.h"
void read_memory_data(FILE *fd, MemoryData *data);
void show_memory_data(MemoryBlock *data);
void resize_memory_block(Window *window, int max_rows, int max_cols);
#endif