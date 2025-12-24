#include "ui/render.h"
void redraw_screen(AppContext *ctx)
{
    // NOT THREAD SAFE, YOU NEED TO LOCK THE MUTEX BEFORE USING IT
    // clear();
    read_memory_data(
        find_proc_file_fd(ctx->proc_files, MEM_INFO),
        &ctx->memory_block.data);
    read_cpuinfo_data(
        find_proc_file_fd(ctx->proc_files, CPU_INFO),
        &ctx->cpu_block.data);
    read_procstat_cpu_data(
        find_proc_file_fd(ctx->proc_files, PROC_STAT),
        &ctx->cpu_block.data);
    read_processes_data(
        find_proc_dir_fd(ctx->proc_files, PROCESSES),
        &ctx->processes_block);
    read_system_uptime(
        find_proc_file_fd(ctx->proc_files, UPTIME), &ctx->cpu_block.data.system_stats);
    read_local_time(&ctx->cpu_block.data.system_stats);

    // show data
    // clean processes' pad
    werase(ctx->processes_block.virtual_pad.itself);
    show_processes(&ctx->processes_block);
    // make sure that there are no gaps between processes in the y
    if (ctx->processes_block.virtual_pad.y >= ctx->processes_block.processes_count)
    {
        // only update real y if selection succeeded
        ctx->processes_block.selected_process_y = ctx->processes_block.processes_count - 1;
        handle_manual_process_selection(&ctx->processes_block);
    }
    else
    {
        refresh_processes_pad(&ctx->processes_block, ctx->processes_block.processes_count);
    }
    show_cpu_data(&ctx->cpu_block);
    show_memory_data(&ctx->memory_block);
}
void *render_routine(void *data)
{
    AppContext *ctx = (AppContext *)data;

    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->mutex);
        while (ctx->is_interacting)
            pthread_cond_wait(&ctx->render_cond, &ctx->mutex);
        ctx->is_rendering = 1; // notify interactivity thread
        pthread_mutex_unlock(&ctx->mutex);

        redraw_screen(ctx);

        pthread_mutex_lock(&ctx->mutex);
        ctx->is_rendering = 0;
        pthread_cond_signal(&ctx->interactivity_cond); // allow interacting
        pthread_mutex_unlock(&ctx->mutex);
        nanosleep(&ctx->render_delay, NULL);
    }
    return NULL;
}
void resize_screen(AppContext *ctx)
{
    // Pad view config
    getmaxyx(stdscr, ctx->max_rows, ctx->max_cols);
    resize_cpu_block(&ctx->cpu_block, ctx->max_rows, ctx->max_cols);
    resize_memory_block(&ctx->memory_block, ctx->max_rows, ctx->max_cols);
    resize_processes_block(&ctx->processes_block, ctx->max_rows, ctx->max_cols);
    // werase(ctx->processes_block.virtual_pad.itself);
    // show_processes(ctx->processes_block);
    // // make sure that there are no gaps between processes in the y
    // if (ctx->processes_block.virtual_pad.y >= ctx->processes_block.processes_count)
    // {
    //     // only update real y if selection succeeded
    //     ctx->processes_block.selected_process_y = ctx->processes_block.processes_count - 1;
    //     handle_manual_process_selection(ctx->processes_block);
    // }
    // else
    // {
    //     refresh_processes_pad(ctx->processes_block, ctx->processes_block.processes_count);
    // }
    // show_cpu_data(ctx->cpu_block);
    // show_memory_data(ctx->memory_block);
    // refresh();
}
