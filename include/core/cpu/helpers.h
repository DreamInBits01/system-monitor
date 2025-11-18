#ifndef CPU_HELPERS_H
#define CPU_HELPERS_H
#include "structs.h"
#include <string.h>
#include <stdlib.h>
void parse_dynamic_cpu_data(char *line, void *data);
void parse_static_cpu_data(char *line, void *data);
void parse_cpu_cores_count(char *line, size_t *output);
#endif