#ifndef PROCESSES_H
#define PROCESSES_H
#include <dirent.h>
#include <stdlib.h>
#define INITIAL_PROCESSES_CAPACITY 20
struct dirent *get_processes(int *count);
#endif