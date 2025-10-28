#ifndef RENDER_H
#define RENDER_H
#include "context.h"
#include "utils.h"
void *render_routine(void *data);
/**
 * redraw_screen - Redraws the entire screen
 *
 * WARNING: This function is NOT thread-safe!
 * CALLER MUST hold ctx->mutex before calling this function.
 *
 * This function directly modifies shared state and calls ncurses functions
 * without internal locking. Calling without proper synchronization will
 * result in race conditions and undefined behavior.
 *
 * @param ctx Application context (must be locked by caller)
 */
void resize_screen(AppContext *ctx);
void redraw_screen(AppContext *ctx);
#endif