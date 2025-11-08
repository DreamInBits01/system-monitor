#include "ui/render.h"
void redraw_screen(AppContext *ctx)
{
    // NOT THREAD SAFE, YOU NEED TO LOCK THE MUTEX BEFORE USING IT
    // clear();
    // read data
    // for (size_t i = 0; i < CACHED_PROC_FILES_NUMBER; i++)
    // {
    //     if (ctx->proc_files[i].is_directory)
    //     {
    //         mvprintw(i, 1, "%s:%x:", ctx->proc_files[i].path, find_proc_dir_fd(ctx->proc_files, ctx->proc_files[i].key));
    //     }
    //     else
    //     {
    //         mvprintw(i, 1, "%s:%x:", ctx->proc_files[i].path, find_proc_file_fd(ctx->proc_files, ctx->proc_files[i].key));
    //     }
    //     /* code */
    // }
    // refresh();
    read_memory_data(
        find_proc_file_fd(ctx->proc_files, "meminfo"),
        &ctx->memory_block->data);
    read_static_cpu_data(
        find_proc_file_fd(ctx->proc_files, "cpuinfo"),
        &ctx->cpu_block->static_data);
    read_dynamic_cpu_data(
        find_proc_file_fd(ctx->proc_files, "cpuinfo"),
        &ctx->cpu_block->dynamic_data);
    read_processes_data(
        find_proc_dir_fd(ctx->proc_files, "processes"),
        ctx->processes_block);

    // clean processes' pad
    werase(ctx->processes_block->virtual_pad.itself);
    show_processes(ctx->processes_block);
    // make sure that there are no gaps between processes in the y
    if (ctx->processes_block->virtual_pad.y >= ctx->processes_block->processes_count)
    {
        // only update real y if selection succeeded
        ctx->processes_block->selected_process_y = ctx->processes_block->processes_count - 1;
        handle_manual_process_selection(ctx);
    }
    else
    {
        refresh_pad(ctx->processes_block, ctx->processes_block->processes_count);
    }
    // show data
    // showing it here temporarily because it has its own window and refreshing the stdscr deletes that
    show_memory_data(ctx->memory_block);
    update_cpu_block(ctx->cpu_block);
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
    ctx->processes_block->virtual_pad.height = (int)(.7 * ctx->max_rows);
    ctx->processes_block->virtual_pad.width = ctx->max_cols;
    prefresh(ctx->processes_block->virtual_pad.itself,
             ctx->processes_block->virtual_pad.y, ctx->processes_block->virtual_pad.x, // Top-left corner of pad content to display (scroll position)
             ctx->processes_block->window.y + 3,                                       // Top edge of screen area where pad appears
             ctx->processes_block->window.x + 2,                                       // Left edge of screen area where pad appears
             ctx->processes_block->window.y + ctx->processes_block->window.height - 2, // Bottom edge of screen area where pad appears
             ctx->processes_block->window.x + ctx->processes_block->window.width - 2); // Right edge of screen area where pad appears
    mvwin(ctx->sort_menu.window, (ctx->max_rows - 15) / 2, (ctx->max_cols - 50) / 2);
}
