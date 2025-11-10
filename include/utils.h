#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include "context/structs.h"
#include "core/processes/structs.h"
FILE *find_proc_file_fd(ProcFile *files, char *key);
DIR *find_proc_dir_fd(ProcFile *files, char *key);
bool proc_dir_read_and_parse(DIR *fd, void (*callback)(struct dirent *ep, void *output), void *output);
bool proc_file_read_and_parse(FILE *fd, void (*callback)(char *line, void *output), void *output);
#endif
