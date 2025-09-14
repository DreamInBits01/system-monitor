#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
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
    WINDOW *itself;
    PadView pad_view;
} PadConfig;
typedef struct
{
    int bar_width;
    size_t *processes_count;
    MemoryInfo *memory_info;
    CpuInfo *cpu_info;
    Process *processes;
    PadConfig pad_config;
    pthread_mutex_t mutex;
} TaskManagerContext;
#endif