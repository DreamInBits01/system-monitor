#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include "structs.h"
#include "helpers.h"
#include "utils.h"
void read_dynamic_cpu_data(FILE *fd, DynamicCpuData *data);
void show_dynamic_cpu_data(CPUBlock *data);
void read_static_cpu_data(FILE *fd, StaticCpuData *data);
void show_static_cpu_data(CPUBlock *data);
void update_cpu_block(CPUBlock *data);
size_t cpu_cores_count(FILE *fd);
#endif