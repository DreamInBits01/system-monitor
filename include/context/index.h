#ifndef CONTEXT_H
#define CONTEXT_H
#include "structs.h"
#include "core/processes/index.h"
#include "core/memory/index.h"
#include "core/cpu/index.h"
#include <fcntl.h>
void initialize_context(AppContext *ctx);
void cleanup_context(AppContext *ctx);
#endif
