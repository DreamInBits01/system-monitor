#ifndef CPU_HELPERS_H
#define CPU_HELPERS_H
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>
#include <math.h>
#define CPU_USAGE_BAR_WIDTH 10

// void parse_dynamic_cpu_data(char *line, void *data);
// void parse_static_cpu_data(char *line, void *data);
void parse_cpuinfo_line(char *line, void *data);
void parse_procstat_cpu_line(char *line, void *data);
void parse_cpu_cores_count(char *line, void *output);
void cpu_usage_bar(WINDOW *window, float fill, int bar_width, int y, int x);
void draw_cpu_window(CPUBlock *cpu_block);
void show_cpu_core_information(CPUBlock *cpu_block, int i, int y, int x);
#endif