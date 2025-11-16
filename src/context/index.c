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
    // sort
    delwin(ctx->sort_menu.window);
    del_panel(ctx->sort_menu.panel);
    // ctx
    pthread_mutex_destroy(&ctx->mutex);
    // pthread_cond_destroy(&ctx->cv);
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
    ctx->sort_menu.window = newwin(15, 50, (ctx->max_rows - 15) / 2, (ctx->max_cols - 50) / 2);
    box(ctx->sort_menu.window, 0, 0);
    mvwaddstr(ctx->sort_menu.window, 1, 1, "Sort");
    // sort panel
    ctx->sort_menu.panel = new_panel(ctx->sort_menu.window);
    ctx->sort_menu.visible = false;
    // ctx mutex
    ctx->running = 1;
    pthread_mutex_init(&ctx->mutex, NULL);
    struct timespec interactivity_delay = {
        .tv_sec = 0,
        .tv_nsec = 25000000};
    struct timespec render_delay = {
        .tv_sec = 2,
        .tv_nsec = 500000000};
    ctx->interactivity_delay = interactivity_delay;
    ctx->render_delay = render_delay;
    // pthread_cond_init(&ctx->cv, NULL);
}