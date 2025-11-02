#ifndef CONTEXT_STRUCTS
#define CONTEXT_STRUCTS
#include <pthread.h>
#include <panel.h>
#include "ui_structs.h"
#include "core/cpu/index.h"
#include "core/processes/structs.h"
#include "core/memory/structs.h"

// Sort menu
typedef struct
{
    WINDOW *window;
    PANEL *panel;
    bool visible;
} SortMenu;
// Processes Pad
typedef struct
{
    WINDOW *itself;
    unsigned height, width, y, x;
} PadView;
typedef struct
{
    WINDOW *itself;
    Process *selected_process;
    PadView pad_view;
    int selected_process_y;
    bool get_process_faild;
    unsigned height, width, x, y;
} PadConfig;

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
    // CPUBlock *cpu_block;
    // ProcessesBlock *processes_block;
    DynamicCpuData *dynamic_cpu_data;
    StaticCpuData *static_cpu_data;
    char **files;
    Process *processes;
    YToPid *y_to_pid;

    pthread_mutex_t mutex;
    PadConfig pad_config;
    SortMenu sort_menu;
    int max_rows;
    int max_cols;
    unsigned bar_width;
    volatile int running;
    unsigned processes_count;
} AppContext;

#endif