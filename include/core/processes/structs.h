#ifndef PROCESSES_STRUCT
#define PROCESSES_STRUCT
#include <sys/types.h>
#include <stdbool.h>
#include "../../third_party/uthash/src/uthash.h"

typedef struct
{
    // 8 bytes
    // char *name;
    char *exe_path;
    char *name;
    double cpu_time;
    double last_uptime;
    UT_hash_handle hh;
    // 4 bytes
    float cpu_usage;
    pid_t pid;
    unsigned y;
    // 1 byte
    char state;
    unsigned char type;
    bool seen;
} Process;
typedef struct
{
    unsigned y;
    pid_t pid;
    bool seen;
    UT_hash_handle hh;
} YToPid;

#endif