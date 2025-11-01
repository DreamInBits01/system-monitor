#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ncurses.h"
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
void read_dynamic_cpu_data(DynamicCpuData *dynamic_cpu_data);
void show_dynamic_cpu_data(DynamicCpuData *dynamic_cpu_data, int max_cols);
void read_static_cpu_data(StaticCpuData *static_cpu_data);
void show_static_cpu_data(StaticCpuData *static_cpu_data, int max_cols);
#endif