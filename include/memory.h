#ifndef MEMORY_H
#define MEMORY_H
#define KB_TO_GB(x) ((x) / (1024 * 1024))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ncurses.h"
#include "utils.h"
typedef struct
{
    // kb
    unsigned buffers;
    // gb
    unsigned cached;
    float total;
    float free;
} MemoryInfo;
MemoryInfo read_memory_info();
void show_memory_info(MemoryInfo *memory_info, int bar_width);
#endif