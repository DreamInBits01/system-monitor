#ifndef CPU_STRUCTS
#define CPU_STRUCTS
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
} StaticCpuData;
typedef struct
{
    StaticCpuData *static_data;
    DynamicCpuData *dynamic_data;
    Window *window;
} CPUBlock;
#endif