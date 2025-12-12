#ifndef CONTEXT_STRUCTS
#define CONTEXT_STRUCTS
#include <pthread.h>
#include <dirent.h>
#include "ui_structs.h"
#include "core/cpu/structs.h"
#include "core/processes/structs.h"
#include "core/memory/structs.h"
#define CACHED_PROC_FILES_NUMBER 5
// Sort menu
/*

UI BLOCKS
-there's a block for each part of the task manager
-each block has a window and data
-data could be static or dynamic or just data
-each block can have a selected item in the window to perform any desired action upon

*/
typedef struct
{
    char *path;
    char *key;
    char *read_mode;
    FILE *fd;
    DIR *dir;
    bool is_directory;
} ProcFile;
typedef struct
{
    MemoryBlock *memory_block;
    CPUBlock *cpu_block;
    ProcessesBlock *processes_block;
    ProcFile proc_files[CACHED_PROC_FILES_NUMBER];
    pthread_mutex_t mutex;
    pthread_cond_t render_cond;
    pthread_cond_t interactivity_cond;
    int max_rows;
    int max_cols;
    unsigned bar_width;
    volatile int running;
    volatile int is_rendering;
    volatile int is_interacting;
    struct timespec render_delay;
    struct timespec interactivity_delay;
} AppContext;

#endif