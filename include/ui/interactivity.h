#ifndef INTERACTIVITY_H
#define INTERACTIVITY_H
#include <signal.h>
#include "ui/render.h"
#include "context.h"
void *interactivity_routine(void *data);
void remove_highlight(PadConfig *pad_config);
void refresh_pad(PadConfig *pad_config, unsigned processes_count);
void update_interactivity_status(PadConfig *pad_config, int processes_count);
#endif