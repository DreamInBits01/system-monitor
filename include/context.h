#ifndef CONTEXT_H
#define CONTEXT_H
#include <ncurses.h>
#include <pthread.h>
#include "core/cpu.h"
#include "core/processes.h"
#include "core/memory.h"
typedef struct
{
    WINDOW *itself;
    int height, width;
    int y, x;
} PadView;
typedef struct
{
    int height, width, x;
    int *y;
    pid_t *selected_process_pid;
    WINDOW *itself;
    PadView pad_view;
    pthread_mutex_t mutex;
} PadConfig;
typedef struct
{
    int bar_width;
    size_t *processes_count;
    MemoryInfo *memory_info;
    DynamicCpuInfo *dynamic_cpu_info;
    StaticCpuInfo *static_cpu_info;
    Process *processes;
    PadConfig pad_config;
    pthread_mutex_t render_mutex;
    volatile int running;
} AppContext;
void initialize_context(AppContext *ctx);
void cleanup_context(AppContext *ctx);
#endif
