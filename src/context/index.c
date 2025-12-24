#include "context/index.h"
void cleanup_context(AppContext *ctx)
{
    ctx->running = 0;
    // fds
    cleanup_cached_fds(ctx->proc_files, CACHED_PROC_FILES_NUMBER);
    cleanup_processes_context(&ctx->processes_block);

    // memory
    cleanup_memory_context(&ctx->memory_block);

    // CPU cleanup
    cleanup_cpu_context(&ctx->cpu_block);

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
    if (!initialize_cpu_context(
            &ctx->cpu_block,
            find_proc_file_fd(ctx->proc_files, "procstat"),
            ctx->max_rows,
            ctx->max_cols))
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
    // ctx mutex
    if (!initialize_threads_context(ctx))
    {
        cleanup_context(ctx);
        exit(1);
    }
    ctx->running = 1;
}