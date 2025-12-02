#ifndef MEMORY_STRUCTS
#define MEMORY_STRUCTS
#include "ui_structs.h"
typedef struct
{
    unsigned long buffers; // kb
    unsigned long cached;  // kb
    float available;       // gb
    float free;            // gb
    float total;           // gb
} MemoryData;
typedef struct
{
    MemoryData data;
    Window window;
    int bar_width;
} MemoryBlock;
#endif