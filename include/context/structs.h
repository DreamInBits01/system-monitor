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

// UI BLOCKS
//  Memory
// Cpu

// Processes
typedef struct
{
    Window *window;
    VirtualPad *virtual_pad;

    // data
    YToPid *y_to_pid;
    Process *processes;
    Process *selected_process;
    int selected_process_y;
    bool get_process_faild;
} ProcessesBlock;
typedef struct
{
    MemoryBlock *memory_block;
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