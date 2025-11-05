#include "context/index.h"
void cleanup_context(AppContext *ctx)
{
    ctx->running = 0;
    if (ctx->processes_block != NULL)
    {
        if (ctx->processes_block->processes != NULL)
        {
            cleanup_processes(&ctx->processes_block->processes);
        }
        if (ctx->processes_block->y_to_pid != NULL)
        {
            cleanup_y_to_pid(&ctx->processes_block->y_to_pid);
            free(ctx->processes_block->y_to_pid);
        }
        delwin(ctx->processes_block->window.itself);
        delwin(ctx->processes_block->virtual_pad.itself);
        free(ctx->processes_block);
    }

    // memory
    if (ctx->memory_block != NULL)
    {
        delwin(ctx->memory_block->window.itself);
        free(ctx->memory_block);
    }
    // CPU cleanup
    if (ctx->cpu_block != NULL)
    {
        free(ctx->cpu_block);
        delwin(ctx->cpu_block->window.itself);
    };
    // pad view
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
    // ctx->y_to_pid = NULL;
    // ctx->processes = NULL;

    ctx->processes_count = 0;
    // Memory config
    ctx->memory_block = malloc(sizeof(MemoryBlock));
    if (ctx->memory_block == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->memory_block, 0, sizeof(MemoryBlock));
    ctx->memory_block->window.height = ctx->max_rows * .25;
    ctx->memory_block->window.width = ctx->max_cols * .48;
    ctx->memory_block->window.itself = newwin(
        ctx->memory_block->window.height,
        ctx->memory_block->window.width,
        ctx->memory_block->window.y,
        ctx->memory_block->window.x);
    ctx->memory_block->bar_width = ctx->max_cols / 4;
    // Cpu config
    ctx->cpu_block = malloc(sizeof(CPUBlock));
    if (ctx->cpu_block == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    memset(ctx->cpu_block, 0, sizeof(CPUBlock));
    ctx->cpu_block->window.height = ctx->max_rows * .25;
    ctx->cpu_block->window.width = ctx->max_cols * .48;
    ctx->cpu_block->window.x = (ctx->max_cols) / 2;
    ctx->cpu_block->window.y = 0;
    ctx->cpu_block->window.itself = newwin(
        ctx->cpu_block->window.height,
        ctx->cpu_block->window.width,
        ctx->cpu_block->window.y,
        ctx->cpu_block->window.x);

    // Processes block
    ctx->processes_block = malloc(sizeof(ProcessesBlock));
    if (ctx->processes_block == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    ctx->processes_block->y_to_pid = NULL;
    ctx->processes_block->processes = NULL;
    // window
    ctx->processes_block->window.height = (int)(.7 * ctx->max_rows);
    ctx->processes_block->window.width = (int)(.55 * ctx->max_cols);
    ctx->processes_block->window.y = (int)(ctx->max_rows * .28);
    ctx->processes_block->window.x = 0;
    ctx->processes_block->window.itself = newwin(
        ctx->processes_block->window.height,
        ctx->processes_block->window.width,
        ctx->processes_block->window.y,
        ctx->processes_block->window.x);
    // virtual pad
    ctx->processes_block->virtual_pad.height = 500;
    ctx->processes_block->virtual_pad.width = 200;
    ctx->processes_block->virtual_pad.itself = newpad(
        ctx->processes_block->virtual_pad.height,
        ctx->processes_block->virtual_pad.width);

    // Pad config

    // ctx->pad_config.height = 500;
    // ctx->pad_config.width = 200;
    // ctx->pad_config.itself = newpad(ctx->pad_config.height, ctx->pad_config.width);
    // ctx->pad_config.y = 0;
    // ctx->pad_config.x = 0;
    // Pad view config
    // ctx->pad_config.pad_view.height = (int)(.7 * ctx->max_rows);
    // ctx->pad_config.pad_view.width = ctx->max_cols;
    // ctx->pad_config.pad_view.y = 10;
    // ctx->pad_config.pad_view.x = 0;
    // ctx->pad_config.pad_view.itself = newwin(
    //     ctx->pad_config.pad_view.height,
    //     ctx->pad_config.pad_view.width,
    //     ctx->pad_config.pad_view.y,
    //     ctx->pad_config.pad_view.x);
    // keypad(ctx->pad_config.pad_view.itself, TRUE);
    // nodelay(ctx->pad_config.pad_view.itself, TRUE);
    // box(ctx->pad_config.pad_view.itself, 0, 0);
    // wrefresh(ctx->pad_config.pad_view.itself);
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
}