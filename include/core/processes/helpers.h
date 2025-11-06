#ifndef PROCESSES_HELPERS
#define PROCESSES_HELPERS
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "structs.h"
#define PROCESS_EXE_PATH_SIZE 30

int is_numeric(char *name);
void cleanup_process(Process *process);
void mark_processes_unseen(Process **processes);
void remove_unseen_processes(Process **processes);
void mark_y_to_pid_unseen(YToPid **y_to_pid);
void remove_y_to_pid_unseen_entries(YToPid **y_to_pid);
void read_process_name(FILE *fd, char **destination);
void read_process_cpu_usage(FILE *fd, Process *found_process);
void read_process_location(char *stat_path, char **destination);
void read_process_stat(char *ep_name, Process *process);
#endif