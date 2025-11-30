#ifndef CPU_HELPERS_H
#define CPU_HELPERS_H
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>
#include <math.h>
#include "helpers.h"
void parse_cpuinfo_line(char *line, void *data);
void parse_procstat_cpu_line(char *line, void *data);
void parse_cpu_cores_count(char *line, void *output);
void cpu_usage_bar(WINDOW *window, float fill, int bar_width, int y, int x);
void draw_cpu_window(CPUBlock *cpu_block);
#endif