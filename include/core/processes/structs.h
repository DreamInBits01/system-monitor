#ifndef PROCESSES_STRUCT
#define PROCESSES_STRUCT
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include "ui_structs.h"
#include "../../third_party/uthash/src/uthash.h"

// UI alignment
#define PROCESSES_PAD_OFFSET 3
//  OFFSET +1 because y is starting from zero.
// Making it +2, will prevent the program from skipping the last element from the previous list
#define PROCESS_PAD_ALIGNMENT 4
// 5 instead of 4 (PROCESSES_PAD_OFFSET+1) To exclude the first element from the next list
#define INITIAL_VIEWPORT_BOTTOM_ALIGNMENT 5
enum SortOption
{
    BY_DEFAULT,
    BY_CPU,
    BY_MEMORY,
    // must be last element
    SORT_OPTS_COUNT
};
typedef struct
{
    // 8 bytes
    char *exe_path;
    char *name;
    char *owner;
    double cpu_time;
    double last_uptime;
    UT_hash_handle hh;
    // 4 bytes
    int threads;
    float cpu_usage;
    pid_t pid;
    unsigned y;
    // 1 byte
    char state;
    unsigned char type;
    bool seen;
    bool is_deleted;
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
    char *sort_options[SORT_OPTS_COUNT];
    enum SortOption sort_option;
    unsigned processes_count;
    int selected_process_y;
    bool get_process_faild;
} ProcessesBlock;
#endif