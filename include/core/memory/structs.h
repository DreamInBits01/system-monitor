#ifndef MEMORY_STRUCTS
#define MEMORY_STRUCTS
typedef struct
{
    unsigned buffers; // kb
    unsigned cached;  // kb
    float free;       // gb
    float total;      // gb
} MemoryData;
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