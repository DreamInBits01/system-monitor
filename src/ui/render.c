#include "context.h"
#include "ui/render.h"
void *render_routine(void *data)
{
    AppContext *ctx = (AppContext *)data;
    pthread_mutex_lock(&ctx->mutex);
    clear();
    read_static_cpu_info(ctx->static_cpu_info);
    show_static_cpu_info(ctx->static_cpu_info);
    pthread_mutex_unlock(&ctx->mutex);
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->mutex);
        read_memory_info(ctx->memory_info);
        show_memory_info(ctx->memory_info, ctx->bar_width);
        read_dynamic_cpu_info(ctx->dynamic_cpu_info);
        show_dynamic_cpu_info(ctx->dynamic_cpu_info);
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
        prefresh(ctx->pad_config.itself,
                 ctx->pad_config.y, ctx->pad_config.x,
                 ctx->pad_config.pad_view.y,
                 ctx->pad_config.pad_view.x,
                 ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                 ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
        pthread_mutex_unlock(&ctx->mutex);
        sleep(3.2);
    }
    return NULL;
}