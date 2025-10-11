#ifndef INTERACTIVITY_H
#define INTERACTIVITY_H
#include <signal.h>
void *interactivity_routine(void *data);
void update_interactivity_status(PadConfig *pad_config, int processes_count);
#endif