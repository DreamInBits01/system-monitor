#ifndef MEMORY_H
#define MEMORY_H
#define KB_TO_GB(x) ((x) / (1024 * 1024))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
typedef struct
{
    unsigned buffers; // kb
    unsigned cached;  // kb
    float free;       // gb
    float total;      // gb
} MemoryInfo;
// typedef struct
// {
//     unsigned buffers; // kb
//     unsigned cached;  // kb
//     float free;       // gb
// } DynamicMemoryInfo;
// typedef struct
// {
//     float total; // gb

// } StaticMemoryInfo;
void read_memory_info(MemoryInfo *memory_info);
void show_memory_info(MemoryInfo *memory_info, int bar_width);
#endif