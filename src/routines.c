#include "routines.h"
void cleanup_context(TaskManagerContext *ctx)
{
    // processes
    free(ctx->processes_count);
    cleanup_processes(&ctx->processes);
    // memory
    free(ctx->memory_info);
    // cpu
    free(ctx->dynamic_cpu_info);
    free(ctx->static_cpu_info);
    // pad
    free(ctx->pad_config.y);
    free(ctx->pad_config.selected_process_pid);
    delwin(ctx->pad_config.itself);
    // pad view
    delwin(ctx->pad_config.pad_view.itself);
    ctx->running = 0;
    pthread_mutex_destroy(&ctx->pad_config.mutex);
    pthread_mutex_destroy(&ctx->render_mutex);
}
void *render_routine(void *data)
{
    TaskManagerContext *ctx = (TaskManagerContext *)data;
    pthread_mutex_lock(&ctx->render_mutex);
    clear();
    read_static_cpu_info(ctx->static_cpu_info);
    show_static_cpu_info(ctx->static_cpu_info);
    pthread_mutex_unlock(&ctx->render_mutex);
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->render_mutex);
        read_memory_info(ctx->memory_info);
        show_memory_info(ctx->memory_info, ctx->bar_width);
        read_dynamic_cpu_info(ctx->dynamic_cpu_info);
        show_dynamic_cpu_info(ctx->dynamic_cpu_info);
        read_processes(&ctx->processes, ctx->processes_count);

        attron(A_BOLD);
        mvprintw(6, 0, "Processes count:%ld", *ctx->processes_count);
        attroff(A_BOLD);
        refresh();
        werase(ctx->pad_config.itself);
        show_processes(&ctx->processes, ctx->pad_config.itself, ctx->pad_config.height, *ctx->pad_config.y);
        pthread_mutex_lock(&ctx->pad_config.mutex);
        prefresh(ctx->pad_config.itself,
                 *ctx->pad_config.y, ctx->pad_config.x,
                 ctx->pad_config.pad_view.y,
                 ctx->pad_config.pad_view.x,
                 ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                 ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
        pthread_mutex_unlock(&ctx->pad_config.mutex);
        pthread_mutex_unlock(&ctx->render_mutex);
        sleep(3.2);
    }
    return NULL;
}
void *interactivity_routine(void *data)
{
    TaskManagerContext *ctx = (TaskManagerContext *)data;
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->render_mutex);
        attron(A_BOLD);
        mvprintw(6, 25, "Scrolled:%.1f%%", (float)*ctx->pad_config.y / (*ctx->processes_count - ctx->pad_config.pad_view.height) * 100);
        mvprintw(6, 45, "Selected process:%d", *ctx->pad_config.selected_process_pid);
        attroff(A_BOLD);
        int ch = getch();
        switch (ch)
        {
        case KEY_F(2):
            kill(*ctx->pad_config.selected_process_pid, SIGKILL);
            break;
        case KEY_F(4):
            goto cleanup;
            break;
        case KEY_UP:
            if (*ctx->pad_config.y > 0)
            {
                pthread_mutex_lock(&ctx->pad_config.mutex);
                *ctx->pad_config.y -= 1;
                pthread_mutex_unlock(&ctx->pad_config.mutex);
                get_selected_process(&ctx->processes, ctx->pad_config.selected_process_pid, *ctx->pad_config.y);
                prefresh(ctx->pad_config.itself,
                         *ctx->pad_config.y, ctx->pad_config.x,
                         ctx->pad_config.pad_view.y,
                         ctx->pad_config.pad_view.x,
                         ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                         ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            }
            break;
        case KEY_DOWN:
            if (*ctx->pad_config.y < *ctx->processes_count - 1)
            {
                pthread_mutex_lock(&ctx->pad_config.mutex);
                *ctx->pad_config.y += 1;
                pthread_mutex_unlock(&ctx->pad_config.mutex);
                get_selected_process(&ctx->processes, ctx->pad_config.selected_process_pid, *ctx->pad_config.y);
                prefresh(ctx->pad_config.itself,
                         *ctx->pad_config.y, ctx->pad_config.x,
                         ctx->pad_config.pad_view.y,
                         ctx->pad_config.pad_view.x,
                         ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                         ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            };
            break;
        case KEY_HOME:
            pthread_mutex_lock(&ctx->pad_config.mutex);
            *ctx->pad_config.y = 0;
            pthread_mutex_unlock(&ctx->pad_config.mutex);
            get_selected_process(&ctx->processes, ctx->pad_config.selected_process_pid, *ctx->pad_config.y);
            prefresh(ctx->pad_config.itself,
                     *ctx->pad_config.y, ctx->pad_config.x,
                     ctx->pad_config.pad_view.y,
                     ctx->pad_config.pad_view.x,
                     ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                     ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            break;
        case KEY_END:
            pthread_mutex_lock(&ctx->pad_config.mutex);
            *ctx->pad_config.y = *ctx->processes_count - 1;
            pthread_mutex_unlock(&ctx->pad_config.mutex);
            get_selected_process(&ctx->processes, ctx->pad_config.selected_process_pid, *ctx->pad_config.y);
            prefresh(ctx->pad_config.itself,
                     *ctx->pad_config.y, ctx->pad_config.x,
                     ctx->pad_config.pad_view.y,
                     ctx->pad_config.pad_view.x,
                     ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                     ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            break;
        }
        pthread_mutex_unlock(&ctx->render_mutex);
        sleep(.5);
    }
cleanup:
    cleanup_context(ctx);
    endwin();
    return NULL;
}
