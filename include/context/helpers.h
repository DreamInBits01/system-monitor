#ifndef CONTEXT_HELPERS_H
#define CONTEXT_HELPERS_H
#include <stdio.h>
#include "core/processes/index.h"
#include "core/processes/structs.h"
#include "core/memory/structs.h"
#include "core/cpu/structs.h"
#include "structs.h"
#include "ncurses.h"
// CACHED FILE DESCRIPTORS
void initialize_fds(ProcFile *destination);
void cleanup_cached_fds(ProcFile *files, size_t size);
// CPU
bool initialize_cpu_context(CPUBlock **data, int max_rows, int max_cols);
void cleanup_cpu_context(CPUBlock *data);
// PROCESSES
bool initialize_processes_context(ProcessesBlock **data, int max_rows, int max_cols);
void cleanup_processes_context(ProcessesBlock *data);
// MEMORY
bool initialize_memory_context(MemoryBlock **data, int max_rows, int max_cols);
void cleanup_memory_context(MemoryBlock *data);
// SORT MENU
bool initialize_sort_menu_context(SortMenu **data, int max_rows, int max_cols);
void cleanup_sort_menu_context(SortMenu *data);
// CTX
bool initialize_threads_context(AppContext *ctx);
void cleanup_threads_context(AppContext *ctx);
#endif