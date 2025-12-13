#ifndef SYSTEM_STRUCTS_H
#define SYSTEM_STRUCTS_H
#include <time.h>
typedef struct
{
    double uptime_in_seconds;
    unsigned uptime_in_hours;
    unsigned uptime_in_minutes;
    double idle_time;
    struct tm *local_time;
} SystemStats;
#endif