#ifndef CONTEXT_H
#define CONTEXT_H
#include <ncurses.h>
#include <pthread.h>
#include "core/cpu.h"
#include "core/processes.h"
#include "core/memory.h"
#include <panel.h>
typedef struct
{
    WINDOW *window;
    PANEL *panel;
    bool visible;

} SortMenu;
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

    unsigned height, width, x, y;
} PadConfig;
typedef struct
{
    MemoryInfo *memory_info;
    DynamicCpuInfo *dynamic_cpu_info;
    StaticCpuInfo *static_cpu_info;
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
void resize_program(AppContext *ctx);
void cleanup_context(AppContext *ctx);
#endif
