#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#define KB_TO_GB(x) ((x) / (1024 * 1024))
typedef struct
{
    // kb
    unsigned buffers;
    // gb
    unsigned cached;
    float total;
    float free;
} MemoryInfo;

void read_memory_info(MemoryInfo *memory_info);
void show_memory_info(MemoryInfo *memory_info, int bar_width);
#endif