#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

#define INITIAL_PROCESSES_CAPACITY 20

// Function declaration - fixed syntax
struct dirent *get_processes(struct dirent *processes, int *count);

#endif