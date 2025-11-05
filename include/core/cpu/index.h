#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
void read_dynamic_cpu_data(DynamicCpuData *data);
void show_dynamic_cpu_data(CPUBlock *data);
void read_static_cpu_data(StaticCpuData *data);
void show_static_cpu_data(CPUBlock *data);
void update_cpu_block(CPUBlock *data);
#endif