#ifndef MAIN_H
#define MAIN_H
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "utils.h"
#include "memory.h"
#include "ncurses.h"
#include "cpu.h"
#include "processes.h"
#include "./third_party/uthash/src/uthash.h"
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
} TaskManagerContext;
#endif