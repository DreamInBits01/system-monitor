#ifndef PROCESSES_STRUCT
#define PROCESSES_STRUCT
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include "ui_structs.h"
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
typedef struct
{
    Window window;
    VirtualPad virtual_pad;

    // data
    YToPid *y_to_pid;
    Process *processes;
    Process *selected_process;
    unsigned processes_count;
    int selected_process_y;
    bool get_process_faild;
} ProcessesBlock;
#endif