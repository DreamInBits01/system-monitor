#ifndef PROCESSES_HELPERS
#define PROCESSES_HELPERS
#include "structs.h"
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#define PROCESS_EXE_PATH_SIZE 1024

int is_numeric(char *name);
void mark_processes_unseen(Process **processes);
void remove_unseen_processes(Process **processes);
void mark_y_to_pid_unseen(YToPid **y_to_pid);
void remove_y_to_pid_unseen_entries(YToPid **y_to_pid);
void read_processes(Process **processes, unsigned *count);
void read_process_cpu_usage(char *ep_name, Process *found_process);
void read_process_location(char *ep_name, char **destination);

#endif