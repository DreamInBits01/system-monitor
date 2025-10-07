#include "context.h"
void cleanup_context(AppContext *ctx)
{
    // processes
    if (ctx->processes_count)
    {
        free(ctx->processes_count);
    }
    if (ctx->processes)
    {
        cleanup_processes(&ctx->processes);
    }
    // memory
    if (ctx->memory_info)
    {
        free(ctx->memory_info);
    }
    // cpu
    if (ctx->dynamic_cpu_info)
    {
        free(ctx->dynamic_cpu_info);
    };
    if (ctx->static_cpu_info)
    {
        free(ctx->static_cpu_info);
    }
    // pad
    if (ctx->pad_config.y)
    {
        free(ctx->pad_config.y);
    }
    if (ctx->pad_config.selected_process_pid)
    {
        free(ctx->pad_config.selected_process_pid);
    }
    delwin(ctx->pad_config.pad_view.itself);
    // pad view
    delwin(ctx->pad_config.itself);
    ctx->running = 0;
    pthread_mutex_destroy(&ctx->pad_config.mutex);
    pthread_mutex_destroy(&ctx->render_mutex);
    endwin();
}
void initialize_context(AppContext *ctx)
{
    //*Todo, add error handling for malloc
    // ctx mutex
    ctx->running = 1;
    pthread_mutex_init(&ctx->render_mutex, NULL);
    // Processes config
    ctx->processes = NULL;
    ctx->processes_count = malloc(sizeof(size_t));
    if (ctx->processes_count == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    *ctx->processes_count = 0;
    // Memory config
    ctx->memory_info = malloc(sizeof(MemoryInfo));
    if (ctx->memory_info == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->memory_info, 0, sizeof(MemoryInfo));
    ctx->bar_width = COLS / 4;
    // Cpu config
    ctx->dynamic_cpu_info = malloc(sizeof(DynamicCpuInfo));
    if (ctx->dynamic_cpu_info == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->dynamic_cpu_info, 0, sizeof(DynamicCpuInfo));
    ctx->static_cpu_info = malloc(sizeof(StaticCpuInfo));
    if (ctx->static_cpu_info == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->static_cpu_info, 0, sizeof(StaticCpuInfo));
    // Pad config
    ctx->pad_config.height = 1000;
    ctx->pad_config.width = 200;
    ctx->pad_config.y = malloc(sizeof(int));
    if (ctx->pad_config.y == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    *ctx->pad_config.y = 0;
    ctx->pad_config.selected_process_pid = malloc(sizeof(pid_t));
    if (ctx->pad_config.selected_process_pid == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    *ctx->pad_config.selected_process_pid = 0;
    ctx->pad_config.itself = newpad(ctx->pad_config.height, ctx->pad_config.width);
    pthread_mutex_init(&ctx->pad_config.mutex, NULL);
    // Pad view config
    ctx->pad_config.pad_view.height = (int)(.7 * LINES);
    ctx->pad_config.pad_view.width = COLS;
    ctx->pad_config.pad_view.y = 8;
    ctx->pad_config.pad_view.x = 0;
    ctx->pad_config.pad_view.itself = newwin(ctx->pad_config.pad_view.height, ctx->pad_config.pad_view.width, *ctx->pad_config.y, ctx->pad_config.pad_view.x);
    // Rendering the pad
    box(ctx->pad_config.pad_view.itself, 0, 0);
    wrefresh(ctx->pad_config.pad_view.itself);
}