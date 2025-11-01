#include "context/index.h"
void cleanup_context(AppContext *ctx)
{
    ctx->running = 0;
    if (ctx->processes != NULL)
    {
        cleanup_processes(&ctx->processes);
        free(ctx->processes);
    }
    if (ctx->y_to_pid != NULL)
    {
        cleanup_y_to_pid(&ctx->y_to_pid);
        free(ctx->y_to_pid);
    }
    // memory
    if (ctx->memory_block != NULL)
    {
        if (ctx->memory_block->data != NULL)
        {
            free(ctx->memory_block->data);
        };
        free(ctx->memory_block);
    }
    // cpu
    if (ctx->dynamic_cpu_data != NULL)
    {
        free(ctx->dynamic_cpu_data);
    };
    if (ctx->static_cpu_data != NULL)
    {
        free(ctx->static_cpu_data);
    }
    // pad view
    delwin(ctx->pad_config.itself);
    delwin(ctx->pad_config.pad_view.itself);
    // sort
    delwin(ctx->sort_menu.window);
    del_panel(ctx->sort_menu.panel);
    // ctx
    pthread_mutex_destroy(&ctx->mutex);
    free(ctx);
    endwin();
}

void initialize_context(AppContext *ctx)
{
    // screen
    getmaxyx(stdscr, ctx->max_rows, ctx->max_cols);
    // char files[5] = {"/proc/cpuData"};
    // ctx->files = files;
    // Processes config
    ctx->y_to_pid = NULL;
    ctx->processes = NULL;
    ctx->processes_count = 0;
    // Memory config
    ctx->memory_block = malloc(sizeof(MemoryBlock));
    if (ctx->memory_block == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->memory_block, 0, sizeof(MemoryBlock));
    ctx->memory_block->window = NULL;
    ctx->memory_block->data = malloc(sizeof(MemoryData));
    if (ctx->memory_block->data == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->memory_block->data, 0, sizeof(MemoryData));
    ctx->bar_width = ctx->max_cols / 4;
    // Cpu config
    ctx->dynamic_cpu_data = malloc(sizeof(DynamicCpuData));
    if (ctx->dynamic_cpu_data == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->dynamic_cpu_data, 0, sizeof(DynamicCpuData));
    ctx->static_cpu_data = malloc(sizeof(StaticCpuData));
    if (ctx->static_cpu_data == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->static_cpu_data, 0, sizeof(StaticCpuData));

    // Pad config
    ctx->pad_config.height = 500;
    ctx->pad_config.width = 200;
    ctx->pad_config.itself = newpad(ctx->pad_config.height, ctx->pad_config.width);
    ctx->pad_config.y = 0;
    // Pad view config
    ctx->pad_config.pad_view.height = (int)(.7 * ctx->max_rows);
    ctx->pad_config.pad_view.width = ctx->max_cols;
    ctx->pad_config.pad_view.y = 8;
    ctx->pad_config.pad_view.x = 0;
    ctx->pad_config.pad_view.itself = newwin(
        ctx->pad_config.pad_view.height,
        ctx->pad_config.pad_view.width,
        ctx->pad_config.pad_view.y,
        ctx->pad_config.pad_view.x);
    box(ctx->pad_config.pad_view.itself, 0, 0);
    wrefresh(ctx->pad_config.pad_view.itself);

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
    // box(ctx->pad_config.pad_view.itself, 0, 0);
    // wrefresh(ctx->pad_config.pad_view.itself);
}