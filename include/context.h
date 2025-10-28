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
    unsigned height, width, x, y;
    WINDOW *itself;
} Window;
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

// Memory block
typedef struct
{
    MemoryInfo *memory_info;
    Window *window
} MemoryBlock;
// Cpu block
typedef struct
{
    StaticCpuInfo *static_cpu_info;
    DynamicCpuInfo *dynamic_cpu_info;
    Window *window
} CpuBlock;
typedef struct
{
    MemoryInfo *memory_info;
    DynamicCpuInfo *dynamic_cpu_info;
    StaticCpuInfo *static_cpu_info;
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
