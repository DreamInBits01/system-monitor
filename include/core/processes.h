#ifndef PROCESSES_H
#define PROCESSES_H

#include "./third_party/uthash/src/uthash.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "ncurses.h"

#define PROCESS_EXE_PATH_SIZE 1024
#define INITIAL_PROCESSES_CAPACITY 20

typedef struct
{
  // 8 bytes
  // char *name;
  char *exe_path;
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
void read_processes(Process **processes, unsigned *count);
void mark_y_to_pid_unseen(YToPid **y_to_pid);
void remove_y_to_pid_unseen_entries(YToPid **y_to_pid);
void mark_processes_unseen(Process **processes);
void remove_unseen_processes(Process **processes, unsigned *processes_count);
void cleanup_processes(Process **processes);
void read_process_cpu_usage(char *ep_name, Process *found_process);
void read_process_location(char *ep_name, char **destination);
void show_processes(Process **processes, YToPid **y_to_pid, WINDOW *pad, unsigned pad_height, unsigned pad_y);
void get_selected_process(YToPid **y_to_pid, pid_t *pid, unsigned target_y);
#endif