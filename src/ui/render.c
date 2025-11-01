#include "ui/render.h"

void redraw_screen(AppContext *ctx)
{
    // NOT THREAD SAFE, YOU NEED TO LOCK THE MUTEX BEFORE USING IT
    clear();
    // read data
    // memory
    read_memory_data(ctx->memory_block->data);
    //  cpu
    read_static_cpu_data(ctx->static_cpu_data);
    read_dynamic_cpu_data(ctx->dynamic_cpu_data);
    // processes
    read_processes(&ctx->processes, &ctx->processes_count);
    // show interactivity metadata
    attron(A_BOLD);
    mvprintw(8, 0, "Processes count:%d", ctx->processes_count);
    mvprintw(8, 25, "Scrolled:%.0f%%", (float)ctx->pad_config.y / (ctx->processes_count - 1) * 100);
    if (ctx->pad_config.selected_process != NULL)
    {
        mvprintw(8, 45, "Selected process:%d, y:%d", ctx->pad_config.selected_process->pid, ctx->pad_config.y);
    }
    attroff(A_BOLD);
    refresh();

    werase(ctx->pad_config.itself);
    show_processes(&ctx->processes, &ctx->y_to_pid, ctx->pad_config.itself, ctx->pad_config.height, ctx->pad_config.y);
    // make sure that there are no gaps between processes in the y
    if (ctx->pad_config.y >= ctx->processes_count)
    {
        // only update real y if selection succeeded
        ctx->pad_config.selected_process_y = ctx->processes_count - 1;
        handle_manual_process_selection(ctx);
    }
    else
    {
        refresh_pad(&ctx->pad_config, ctx->processes_count);
    }
    // show data
    // showing it here temporarily because it has its own window and refreshing the stdscr deletes that
    show_memory_data(ctx->memory_block);
    show_static_cpu_data(ctx->static_cpu_data, ctx->max_cols);
    show_dynamic_cpu_data(ctx->dynamic_cpu_data, ctx->max_cols);
}
void *render_routine(void *data)
{
    AppContext *ctx = (AppContext *)data;
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->mutex);
        redraw_screen(ctx);
        pthread_mutex_unlock(&ctx->mutex);
        sleep(4);
    }
    return NULL;
}
void resize_screen(AppContext *ctx)
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
