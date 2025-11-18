#ifndef CPU_STRUCTS_H
#define CPU_STRUCTS_H
#include "ui_structs.h"
typedef struct
{
    float avg_mhz;
} DynamicCpuData;
typedef struct
{
    int logical_cpus;
    int physical_cores;
    char model_name[128];
    size_t cpu_cores_count;
} StaticCpuData;
typedef struct
{
    StaticCpuData static_data;
    DynamicCpuData dynamic_data;
    Window window;
} CPUBlock;

#endif