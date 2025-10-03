#ifndef ROUTINES_H
#define ROUTINES_H
#include <signal.h>
#include "context.h"
void *render_routine(void *data);
void *interactivity_routine(void *data);
void cleanup_context(TaskManagerContext *ctx);
#endif