#ifndef MEMORY_H
#define MEMORY_H
#define KB_TO_GB(x) ((x) / (1024 * 1024))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "structs.h"
#include "helpers.h"
#include "context/structs.h"
void read_memory_data(MemoryData *memory_data);
void show_memory_data(MemoryBlock *memory_block);
#endif