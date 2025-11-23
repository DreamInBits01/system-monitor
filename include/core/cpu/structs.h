#ifndef CPU_STRUCTS_H
#define CPU_STRUCTS_H
#include "ui_structs.h"
typedef struct
{
    int usage;
    int heat;
    double active_time;
    double idle_time;
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
    int cpu_cores_count;
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
