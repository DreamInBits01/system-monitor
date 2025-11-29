#ifndef PROCESSES_HELPERS
#define PROCESSES_HELPERS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "structs.h"
#define PROCESS_EXE_PATH_SIZE 30

int is_numeric(char *name);
// Read functions
void read_process_name(FILE *fd, char **destination);
void read_process_cpu_usage(FILE *fd, Process *found_process);
void read_process_location(char *stat_path, char **destination);
void read_process_stat(char *ep_name, Process *process);

// Processes state housekeeping
void mark_processes_unseen(Process **processes);
void remove_unseen_processes(Process **processes);
void mark_y_to_pid_unseen(YToPid **y_to_pid);
void remove_y_to_pid_unseen_entries(YToPid **y_to_pid);

// Parsing
void parse_processes_dir(struct dirent *ep, void *data);
void parse_process_status_line(char *line, void *output);

// Cleanup
void cleanup_process(Process *process);

// UI
void draw_processes_window(ProcessesBlock *data);
void highlight_process(ProcessesBlock *data);
void update_interactivity_metadata(ProcessesBlock *data, int processes_count);

#endif