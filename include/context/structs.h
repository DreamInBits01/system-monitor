#ifndef CONTEXT_STRUCTS
#define CONTEXT_STRUCTS
#include <pthread.h>
#include "ui_structs.h"
#include "core/cpu/index.h"
#include "core/processes/structs.h"
#include "core/memory/structs.h"

// Sort menu
/*
UI BLOCKS
-there's a block for each part of the task manager
-each block has a window and data
-data could be static or dynamic or just data
-each block can have a selected item in the window to perform any desired action upon
*/
typedef struct
{
    MemoryBlock *memory_block;
    CPUBlock *cpu_block;
    ProcessesBlock *processes_block;
    char **files;
    pthread_mutex_t mutex;
    SortMenu sort_menu;
    int max_rows;
    int max_cols;
    unsigned bar_width;
    volatile int running;
    unsigned processes_count;
} AppContext;

#endif