#include "context.h"
#include "ui/interactivity.h"
void *interactivity_routine(void *data)
{
    AppContext *ctx = (AppContext *)data;
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->render_mutex);
        attron(A_BOLD);
        mvprintw(6, 25, "Scrolled:%.0f%%", (float)*ctx->pad_config.y / (*ctx->processes_count - 1) * 100);
        mvprintw(6, 45, "Selected process:%d", *ctx->pad_config.selected_process_pid);
        attroff(A_BOLD);
        int ch = getch();
        switch (ch)
        {
        case KEY_F(2):
            if (*ctx->pad_config.selected_process_pid == 0)
                return;
            int kill_result = kill(*ctx->pad_config.selected_process_pid, SIGKILL);
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
    // endwin();
    return NULL;
}