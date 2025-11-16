#include "context/index.h"
void cleanup_context(AppContext *ctx)
{
    ctx->running = 0;
    // fds
    cleanup_cached_fds(ctx->proc_files, CACHED_PROC_FILES_NUMBER);
    if (ctx->processes_block != NULL)
        cleanup_processes_context(ctx->processes_block);

    // memory
    if (ctx->memory_block != NULL)
        cleanup_memory_context(ctx->memory_block);

    // CPU cleanup
    if (ctx->cpu_block != NULL)
        cleanup_cpu_context(ctx->cpu_block);

    if (ctx->sort_menu != NULL)
    {
        cleanup_sort_menu_context(ctx->sort_menu);
    }
    // ctx
    cleanup_threads_context(ctx);
    free(ctx);
    endwin();
}
void initialize_context(AppContext *ctx)
{
    // screen
    getmaxyx(stdscr, ctx->max_rows, ctx->max_cols);
    initialize_fds(ctx->proc_files);
    // Memory conf
    if (!initialize_memory_context(&ctx->memory_block, ctx->max_rows, ctx->max_cols))
    {
        cleanup_context(ctx);
        exit(1);
    }
    // CPU config
    if (!initialize_cpu_context(&ctx->cpu_block, ctx->max_rows, ctx->max_cols))
    {
        cleanup_context(ctx);
        exit(1);
    }
    // Processes block
    if (!initialize_processes_context(&ctx->processes_block, ctx->max_rows, ctx->max_cols))
    {
        cleanup_context(ctx);
        exit(1);
    }
    // Sort menu
    if (!initialize_sort_menu_context(&ctx->sort_menu, ctx->max_rows, ctx->max_cols))
    {
        cleanup_context(ctx);
        exit(1);
    }
    // ctx mutex
    if (!initialize_threads_context(ctx))
    {
        cleanup_context(ctx);
        exit(1);
    }
    ctx->running = 1;
}