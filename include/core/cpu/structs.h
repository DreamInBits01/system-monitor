#ifndef CPU_STRUCTS_H
#define CPU_STRUCTS_H
#include "ui_structs.h"
typedef struct
{
    size_t usage;
    size_t heat;
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
    char model_name[128];
    size_t cpu_cores_count;
    float avg_mhz;
    CPUCore *cores;
} CPUData;
typedef struct
{
    CPUData data;
    Window window;
} CPUBlock;
#endif
