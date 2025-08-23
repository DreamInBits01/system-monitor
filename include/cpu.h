#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ncurses.h"
typedef struct
{
    int logical_cpus;
    int physical_cores;
    int mhz_occurrence;
    float total_mhz;
    float avg_mhz;
    char model_name[128];

} CpuInfo;
CpuInfo read_cpu_info();
void show_cpu_info(CpuInfo *cpu_info);
#endif