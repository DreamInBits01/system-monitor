#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include "structs.h"
#include "helpers.h"
#include "utils.h"
void read_cpuinfo_data(FILE *fd, CPUData *data);
void show_cpuinfo_data(CPUBlock *cpu_block);
// void update_cpu_block(CPUBlock *data);
size_t cpu_cores_count(FILE *fd);
#endif