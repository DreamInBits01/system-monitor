#include "context.h"
void cleanup_context(AppContext *ctx)
{
    ctx->running = 0;
    if (ctx->processes)
    {
        cleanup_processes(&ctx->processes);
    }
    // memory
    if (ctx->memory_info != NULL)
    {
        free(ctx->memory_info);
    }
    // cpu
    if (ctx->dynamic_cpu_info != NULL)
    {
        free(ctx->dynamic_cpu_info);
    };
    if (ctx->static_cpu_info != NULL)
    {
        free(ctx->static_cpu_info);
    }
    // pad view
    delwin(ctx->pad_config.itself);
    delwin(ctx->pad_config.pad_view.itself);
    pthread_mutex_destroy(&ctx->mutex);
    free(ctx);
    endwin();
}
void resize_program(AppContext *ctx)
{
    // Pad view config
    getmaxyx(stdscr, ctx->max_rows, ctx->max_cols);
    clear();
    ctx->pad_config.pad_view.height = (int)(.7 * ctx->max_rows);
    ctx->pad_config.pad_view.width = ctx->max_cols;
    prefresh(ctx->pad_config.itself,
             ctx->pad_config.y, ctx->pad_config.x,                             // Top-left corner of pad content to display (scroll position)
             ctx->pad_config.pad_view.y,                                       // Top edge of screen area where pad appears
             ctx->pad_config.pad_view.x,                                       // Left edge of screen area where pad appears
             ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1, // Bottom edge of screen area where pad appears
             ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1); // Right edge of screen area where pad appears
    mvwin(ctx->sort_menu.window, (ctx->max_rows - 15) / 2, (ctx->max_cols - 50) / 2);
}
void initialize_context(AppContext *ctx)
{
    // screen
    getmaxyx(stdscr, ctx->max_rows, ctx->max_cols);
    // Processes config
    ctx->y_to_pid = NULL;
    ctx->processes = NULL;
    ctx->processes_count = 0;
    // Memory config
    ctx->memory_info = malloc(sizeof(MemoryInfo));
    if (ctx->memory_info == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->memory_info, 0, sizeof(MemoryInfo));
    ctx->bar_width = ctx->max_cols / 4;
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
    ctx->pad_config.height = 500;
    ctx->pad_config.width = 200;
    ctx->pad_config.itself = newpad(ctx->pad_config.height, ctx->pad_config.width);
    // Pad view config
    ctx->pad_config.pad_view.height = (int)(.7 * ctx->max_rows);
    ctx->pad_config.pad_view.width = ctx->max_cols;
    ctx->pad_config.pad_view.y = 8;
    ctx->pad_config.pad_view.itself = newwin(
        ctx->pad_config.pad_view.height,
        ctx->pad_config.pad_view.width,
        ctx->pad_config.y,
        ctx->pad_config.pad_view.x);
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
    // Rendering the pad
    box(ctx->pad_config.pad_view.itself, 0, 0);
    wrefresh(ctx->pad_config.pad_view.itself);
}