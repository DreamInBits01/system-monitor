#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ncurses.h"
typedef struct
{
    float avg_mhz;
} DynamicCpuInfo;
typedef struct
{
    int logical_cpus;
    int physical_cores;
    char model_name[128];
} StaticCpuInfo;
void read_dynamic_cpu_info(DynamicCpuInfo *dynamic_cpu_info);
void show_dynamic_cpu_info(DynamicCpuInfo *dynamic_cpu_info);
void read_static_cpu_info(StaticCpuInfo *static_cpu_info);
void show_static_cpu_info(StaticCpuInfo *static_cpu_info);
#endif