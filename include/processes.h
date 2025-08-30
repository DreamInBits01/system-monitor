#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdbool.h>
#include "./third_party/uthash/src/uthash.h"
#define INITIAL_PROCESSES_CAPACITY 20

typedef struct
{
    unsigned pid;
    char *name;
    unsigned char type;
    bool seen;
    char state;
    UT_hash_handle hh;
} Process;
// Function declaration - fixed syntax
void get_processes(Process **processes, size_t *count);
void mark_processes_unseen(Process **processes);
void remove_unseen_processes(Process **processes, size_t *processes_count);
#endif