#ifndef SYSTEM_STRUCTS_H
#define SYSTEM_STRUCTS_H
#include <time.h>
typedef struct
{
    double uptime;
    double idle_time;
    struct tm *local_time;
} SystemStats;
#endif