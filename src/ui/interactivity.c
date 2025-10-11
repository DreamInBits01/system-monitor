#include "context.h"
#include "ui/interactivity.h"
void update_interactivity_status(AppContext *ctx)
{
    move(6, 25);
    clrtoeol();
    attron(A_BOLD);
    mvprintw(6, 25, "Scrolled:%.0f%%", (float)ctx->pad_config.y / (ctx->processes_count - 1) * 100);
    if (ctx->pad_config.selected_process != NULL)
    {
        mvprintw(6, 45, "Selected process:%d, y:%d", ctx->pad_config.selected_process->pid, ctx->pad_config.y);
    }
    attroff(A_BOLD);
    refresh();
}
void *interactivity_routine(void *data)
{
    AppContext *ctx = (AppContext *)data;
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->render_mutex);
        int ch = getch();
        switch (ch)
        {
        case KEY_F(2):
            if (ctx->pad_config.selected_process == NULL)
                return NULL;
            if (ctx->pad_config.selected_process->pid == 0)
                return;
            int kill_result = kill(ctx->pad_config.selected_process->pid, SIGKILL);
            if (kill_result == -1)
            {
                perror("Kill faild!\n");
            }
            goto cleanup;
            break;
        case KEY_F(4):
            goto cleanup;
            break;
        case KEY_UP:
            if (ctx->pad_config.y > 0)
            {
                pthread_mutex_lock(&ctx->pad_config.mutex);
                ctx->pad_config.y -= 1;
                pthread_mutex_unlock(&ctx->pad_config.mutex);
                // remove highlighting
                if (ctx->pad_config.selected_process != NULL)
                {
                    mvwprintw(ctx->pad_config.itself, ctx->pad_config.selected_process->y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
                }
                get_selected_process(&ctx->processes, &ctx->y_to_pid, &ctx->pad_config.selected_process, ctx->pad_config.y);

                wattron(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
                mvwprintw(ctx->pad_config.itself, ctx->pad_config.y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
                wattroff(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);

                prefresh(ctx->pad_config.itself,
                         ctx->pad_config.y, ctx->pad_config.x,
                         ctx->pad_config.pad_view.y,
                         ctx->pad_config.pad_view.x,
                         ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                         ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
                update_interactivity_status(ctx);
            }
            break;
        case KEY_DOWN:
            if (ctx->pad_config.y < ctx->processes_count - 1)
            {
                pthread_mutex_lock(&ctx->pad_config.mutex);
                ctx->pad_config.y += 1;
                pthread_mutex_unlock(&ctx->pad_config.mutex);
                // remove highlighting

                if (ctx->pad_config.selected_process != NULL)
                {
                    mvwprintw(ctx->pad_config.itself, ctx->pad_config.selected_process->y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
                }
                get_selected_process(&ctx->processes, &ctx->y_to_pid, &ctx->pad_config.selected_process, ctx->pad_config.y);

                // you need to turn off the styles of the process before this one
                mvwprintw(ctx->pad_config.itself, ctx->pad_config.y - 1, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
                wattron(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
                mvwprintw(ctx->pad_config.itself, ctx->pad_config.y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
                wattroff(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
                prefresh(ctx->pad_config.itself,
                         ctx->pad_config.y, ctx->pad_config.x,
                         ctx->pad_config.pad_view.y,
                         ctx->pad_config.pad_view.x,
                         ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                         ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
                update_interactivity_status(ctx);
            };
            break;
        case KEY_HOME:
            pthread_mutex_lock(&ctx->pad_config.mutex);
            ctx->pad_config.y = 0;
            pthread_mutex_unlock(&ctx->pad_config.mutex);
            // remove highlighting

            if (ctx->pad_config.selected_process != NULL)
            {
                mvwprintw(ctx->pad_config.itself, ctx->pad_config.selected_process->y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
            }
            get_selected_process(&ctx->processes, &ctx->y_to_pid, &ctx->pad_config.selected_process, ctx->pad_config.y);
            wattron(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            mvwprintw(ctx->pad_config.itself, ctx->pad_config.y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
            wattroff(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            prefresh(ctx->pad_config.itself,
                     ctx->pad_config.y, ctx->pad_config.x,
                     ctx->pad_config.pad_view.y,
                     ctx->pad_config.pad_view.x,
                     ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                     ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            update_interactivity_status(ctx);

            break;
        case KEY_END:
            pthread_mutex_lock(&ctx->pad_config.mutex);
            ctx->pad_config.y = ctx->processes_count - 1;
            pthread_mutex_unlock(&ctx->pad_config.mutex);
            // remove highlighting

            if (ctx->pad_config.selected_process != NULL)
            {
                mvwprintw(ctx->pad_config.itself, ctx->pad_config.selected_process->y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
            }
            get_selected_process(&ctx->processes, &ctx->y_to_pid, &ctx->pad_config.selected_process, ctx->pad_config.y);
            wattron(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            mvwprintw(ctx->pad_config.itself, ctx->pad_config.y, 0, "Process: %d, %s, cpu_usage:%.2f%%", ctx->pad_config.selected_process->pid, ctx->pad_config.selected_process->exe_path, ctx->pad_config.selected_process->cpu_usage);
            wattroff(ctx->pad_config.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            prefresh(ctx->pad_config.itself,
                     ctx->pad_config.y, ctx->pad_config.x,
                     ctx->pad_config.pad_view.y,
                     ctx->pad_config.pad_view.x,
                     ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                     ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            update_interactivity_status(ctx);

            break;
        }
        pthread_mutex_unlock(&ctx->render_mutex);
        sleep(.75);
    }
cleanup:
    cleanup_context(ctx);
    // endwin();
    return NULL;
}