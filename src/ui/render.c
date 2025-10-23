#include "ui/render.h"
void redraw_screen(AppContext *ctx)
{
    pthread_mutex_lock(&ctx->mutex);
    // flicker
    clear();

    // memory info
    read_memory_info(ctx->memory_info);
    show_memory_info(ctx->memory_info, ctx->bar_width);
    // cpu info
    read_static_cpu_info(ctx->static_cpu_info);
    show_static_cpu_info(ctx->static_cpu_info, ctx->max_cols);

    read_dynamic_cpu_info(ctx->dynamic_cpu_info);
    show_dynamic_cpu_info(ctx->dynamic_cpu_info, ctx->max_cols);
    // processes info
    read_processes(&ctx->processes, &ctx->processes_count);
    attron(A_BOLD);
    mvprintw(6, 0, "Processes count:%d", ctx->processes_count);
    mvprintw(6, 25, "Scrolled:%.0f%%", (float)ctx->pad_config.y / (ctx->processes_count - 1) * 100);
    if (ctx->pad_config.selected_process != NULL)
    {
        mvprintw(6, 45, "Selected process:%d, y:%d", ctx->pad_config.selected_process->pid, ctx->pad_config.y);
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

    if (ctx->sort_menu.visible)
    {
        top_panel(ctx->sort_menu.panel);
        // Force panel redraw
        touchwin(ctx->sort_menu.window);
        update_panels();
        doupdate();
    }

    pthread_mutex_unlock(&ctx->mutex);
}
void *render_routine(void *data)
{
    AppContext *ctx = (AppContext *)data;
    while (ctx->running)
    {
        redraw_screen(ctx);
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
