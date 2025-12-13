#ifndef CPU_STRUCTS_H
#define CPU_STRUCTS_H
#include "ui_structs.h"
#include "core/system/structs.h"
typedef struct
{
    double usage;
    int heat;
    unsigned long long prev_active_time;
    unsigned long long prev_total_time;
} CPUCore;
typedef struct
{
    int logical_cpus;
    int physical_cores;
    int cpu_cores_count;
    int avg_mhz;
    char model_name[32];
    double cpu_usage;
    unsigned long long prev_active_time;
    unsigned long long prev_total_time;
    SystemStats system_stats;
    CPUCore *cores;
} CPUData;
typedef struct
{
    CPUData data;
    Window window;
} CPUBlock;
#endif
