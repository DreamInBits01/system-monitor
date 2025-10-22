#ifndef RENDER_H
#define RENDER_H
#include "context.h"
#include "utils.h"
void *render_routine(void *data);
void resize_screen(AppContext *ctx);
void redraw_screen(AppContext *ctx);
#endif