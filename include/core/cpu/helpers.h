#ifndef CPU_HELPERS_H
#define CPU_HELPERS_H
#include "structs.h"
#include <string.h>
void parse_dynamic_cpu_data(char *line, void *data);
void parse_static_cpu_data(char *line, void *data);
#endif