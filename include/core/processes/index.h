#ifndef PROCESSES_H
#define PROCESSES_H

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "ncurses.h"
#include "structs.h"
#include "helpers.h"
#include "utils.h"
// #include "utils.h"
#define INITIAL_PROCESSES_CAPACITY 20
void read_processes(Process **processes, unsigned *count);
// void show_processes(Process **processes, YToPid **y_to_pid, WINDOW *pad, unsigned pad_height, unsigned pad_y);
void show_processes(ProcessesBlock *data);
void get_selected_process(Process **processes, YToPid **y_to_pid, Process **output, bool *get_process_faild, unsigned target_y);
void cleanup_processes(Process **processes);
void cleanup_y_to_pid(YToPid **y_to_pid);
#endif