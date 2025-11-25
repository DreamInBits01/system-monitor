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
// typedef struct
// {
//     float avg_mhz;
//     CPUCore *cores;
// } DynamicCpuData;
// typedef struct
// {
//     int logical_cpus;
//     int physical_cores;
//     char model_name[128];
//     size_t cpu_cores_count;
// } StaticCpuData;
typedef struct
{
    int logical_cpus;
    int physical_cores;
    unsigned cpu_cores_count;
    int avg_mhz;
    char model_name[32];
    CPUCore *cores;
} CPUData;
typedef struct
{
    CPUData data;
    Window window;
} CPUBlock;
#endif
