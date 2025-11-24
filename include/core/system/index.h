#ifndef SYSTEM_H
#define SYSTEM_H
#include <time.h>
#include "utils.h"
#include "structs.h"
void read_system_uptime(FILE *fd, SystemStats *data);
void read_local_time(SystemStats *data);
void show_local_time(Window *window, struct tm *local_time);
#endif