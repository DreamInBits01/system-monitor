#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include "structs.h"
#include "helpers.h"
#include "utils.h"
#include <math.h>
#include <time.h>
void read_cpuinfo_data(FILE *fd, CPUData *data);
// void show_cpuinfo_data(CPUBlock *cpu_block);
void show_cpu_data(CPUBlock *cpu_block);
void read_procstat_cpu_data(FILE *fd, CPUData *data);
size_t cpu_cores_count(FILE *fd);
#endif