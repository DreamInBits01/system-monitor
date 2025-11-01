#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <ncurses.h>
#include "context/index.h"
void remove_process_highlight(PadConfig *pad_config);
void refresh_pad(PadConfig *pad_config, unsigned processes_count);
void update_interactivity_metadata(PadConfig *pad_config, int processes_count);
void highlight_process(PadConfig *pad_config);
void handle_manual_process_selection(AppContext *ctx);
void show_process_information(Process *process, WINDOW *pad, int y);
#endif
