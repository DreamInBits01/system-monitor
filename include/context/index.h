#ifndef CONTEXT_H
#define CONTEXT_H
#include "structs.h"
#include "helpers.h"
#include <fcntl.h>
void initialize_context(AppContext *ctx);
void cleanup_context(AppContext *ctx);
#endif
