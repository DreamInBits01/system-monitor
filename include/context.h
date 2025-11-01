#ifndef CONTEXT_H
#define CONTEXT_H
#include <ncurses.h>
#include <pthread.h>
#include <panel.h>
#include "core/cpu/index.h"
#include "core/processes/index.h"
#include "core/memory/index.h"

// Screen Block
typedef struct
{
    WINDOW *itself;
    unsigned height, width, x, y;
    bool hidden;
} Window;
typedef struct
{
    /*
        -This is where large content is put
        -It must have a regular window that acts as a viewing frame
    */
    WINDOW *itself;
    unsigned height, width, y, x;
} VirtualPad;

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
typedef struct
{
    MemoryData *data;
    Window *window;
} MemoryBlock;
// Cpu
typedef struct
{
    StaticCpuData *static_data;
    DynamicCpuData *dynamic_data;
    Window *window;
} CpuBlock;

// Processes
typedef struct
{
    Process *processes;
    YToPid *y_to_pid;
    Window *window;
    VirtualPad *virtual_pad;

    // selected process data
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
void initialize_context(AppContext *ctx);
void cleanup_context(AppContext *ctx);
#endif
