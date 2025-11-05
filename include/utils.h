#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <ncurses.h>
#include "context/index.h"
void remove_process_highlight(ProcessesBlock *data);
void refresh_pad(ProcessesBlock *data, unsigned processes_count);
void update_interactivity_metadata(ProcessesBlock *data, int processes_count);
void highlight_process(ProcessesBlock *data);
void handle_manual_process_selection(AppContext *ctx);
void show_process_information(Process *process, WINDOW *virtual_pad, int y);
#endif
