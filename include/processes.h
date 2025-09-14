#ifndef PROCESSES_H
#define PROCESSES_H

#include "./third_party/uthash/src/uthash.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "ncurses.h"
#define PROCESS_EXE_PATH_SIZE 1024
#define INITIAL_PROCESSES_CAPACITY 20

typedef struct
{
  unsigned pid;
  unsigned char type;
  bool seen;
  char state;
  char *name;
  char *exe_path;
  UT_hash_handle hh;
} Process;
void read_processes(Process **processes, size_t *count);
void mark_processes_unseen(Process **processes);
void remove_unseen_processes(Process **processes, size_t *processes_count);
void cleanup_processes(Process **processes);
void read_process_stat(char *ep_name, char *destination);
void read_process_location(char *ep_name, char **destination);
void show_processes(Process **processes, WINDOW *pad, int pad_height, int pad_y);
#endif