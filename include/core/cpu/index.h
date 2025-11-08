#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include "structs.h"
#include "helpers.h"
#include "utils.h"
void read_dynamic_cpu_data(int fd, DynamicCpuData *data);
void show_dynamic_cpu_data(CPUBlock *data);
void read_static_cpu_data(int fd, StaticCpuData *data);
void show_static_cpu_data(CPUBlock *data);
void update_cpu_block(CPUBlock *data);
#endif