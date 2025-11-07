#ifndef MEMORY_STRUCTS
#define MEMORY_STRUCTS
#include "ui_structs.h"
typedef struct
{
    unsigned long buffers; // kb
    unsigned long cached;  // kb
    unsigned available;    // gb
    float free;            // gb
    float total;           // gb
} MemoryData;
typedef struct
{
    MemoryData data;
    Window window;
    int bar_width;
} MemoryBlock;

// typedef struct
// {
//     unsigned buffers; // kb
//     unsigned cached;  // kb
//     float free;       // gb
// } DynamicMemoryData;
// typedef struct
// {
//     float total; // gb

// } StaticMemoryData;
#endif