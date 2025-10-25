#ifndef MEMORY_STRUCTS
#define MEMORY_STRUCTS
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
#endif