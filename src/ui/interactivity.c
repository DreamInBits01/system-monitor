#include "context.h"
#include "ui/interactivity.h"
/*
    TOOODOODOO
        Undoes what happens when get_selected_process keeps the same process even when
        it doesn't find anything

    Selected process is only null initially

    1-Use a switch case to decide the case
    2-When moving in the pad, check the boundries if you're going up or down
    3-remove highlighting if there's a selected process
    4-update the selected_process id (drived from the main pad_config's y)
    5-on success, update the pad_config's y and do the highlighting routine
*/
void update_interactivity_status(PadConfig *pad_config, int processes_count)
{
    move(6, 25);
    clrtoeol();
    attron(A_BOLD);
    mvprintw(6, 25, "Scrolled:%.0f%%", (float)pad_config->y / (processes_count - 1) * 100);
    if (pad_config->selected_process != NULL)
    {
        mvprintw(6, 45, "Selected process:%d, y:%d", pad_config->selected_process->pid, pad_config->y);
    }
    attroff(A_BOLD);
    refresh();
}
void remove_highlight(PadConfig *pad_config)
{
    if (pad_config->selected_process == NULL)
        return NULL;
    mvwprintw(pad_config->itself, pad_config->selected_process->y, 0, "Process: %d, %s, cpu_usage:%.2f%%",
              pad_config->selected_process->pid,
              pad_config->selected_process->exe_path,
              pad_config->selected_process->cpu_usage);
}
void highlight_process(PadConfig *pad_config)
{
    wattron(pad_config->itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
    mvwprintw(pad_config->itself, pad_config->y, 0, "Process: %d, %s, cpu_usage:%.2f%%", pad_config->selected_process->pid, pad_config->selected_process->exe_path, pad_config->selected_process->cpu_usage);
    wattroff(pad_config->itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
}
void refresh_pad(PadConfig *pad_config)
{
    prefresh(pad_config->itself,
             pad_config->y, pad_config->x,
             pad_config->pad_view.y,
             pad_config->pad_view.x,
             pad_config->pad_view.y + pad_config->pad_view.height - 1,
             pad_config->pad_view.x + pad_config->pad_view.width - 1);
}
void *interactivity_routine(void *data)
{
    AppContext *ctx = (AppContext *)data;
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->mutex);
        int ch = getch();

        switch (ch)
        {
        case KEY_RESIZE:
            resize_screen(ctx);
            redraw_screen(ctx);
            break;
        case KEY_F(1):
            goto cleanup;
            break;
        case KEY_F(2):
            if (ctx->pad_config.selected_process == NULL)
                return NULL;
            if (ctx->pad_config.selected_process->pid == 0)
                return NULL;
            int kill_result = kill(ctx->pad_config.selected_process->pid, SIGKILL);
            if (kill_result == -1)
            {
                perror("Kill faild!\n");
            }
            if (ctx->pad_config.selected_process->pid == getpid())
            {
                goto cleanup;
            }
            break;

        case KEY_F(3):
            if (ctx->sort_menu.visible)
            {
                hide_panel(ctx->sort_menu.panel);
                update_panels();
                doupdate();
            }
            else
            {
                top_panel(ctx->sort_menu.panel);
                update_panels();
                doupdate();
            }
            ctx->sort_menu.visible = !ctx->sort_menu.visible;
            break;

        case KEY_UP:
            if (ctx->pad_config.y > 0 && ctx->sort_menu.visible == false)
            {
                // remove highlighting
                remove_highlight(&ctx->pad_config);
                // next process's y

                ctx->pad_config.selected_process_y = ctx->pad_config.y - 1;
                // get new process
                get_selected_process(
                    &ctx->processes,
                    &ctx->y_to_pid,
                    &ctx->pad_config.selected_process,
                    &ctx->pad_config.get_process_faild,
                    ctx->pad_config.selected_process_y);
                // add highlighting if successful
                if (!ctx->pad_config.get_process_faild)
                {
                    ctx->pad_config.y = ctx->pad_config.selected_process_y;
                    highlight_process(&ctx->pad_config);
                    refresh_pad(&ctx->pad_config);
                    update_interactivity_status(&ctx->pad_config, ctx->processes_count);
                }
            }
            break;
        case KEY_DOWN:
            if (ctx->pad_config.y < ctx->processes_count - 1 && ctx->sort_menu.visible == false)
            {
                // remove highlighting
                remove_highlight(&ctx->pad_config);
                // next process's y
                ctx->pad_config.selected_process_y = ctx->pad_config.y + 1;
                // get new process
                get_selected_process(
                    &ctx->processes,
                    &ctx->y_to_pid,
                    &ctx->pad_config.selected_process,
                    &ctx->pad_config.get_process_faild,
                    ctx->pad_config.selected_process_y);
                // add highlighting if successful
                if (!ctx->pad_config.get_process_faild)
                {
                    ctx->pad_config.y = ctx->pad_config.selected_process_y;
                    highlight_process(&ctx->pad_config);
                    refresh_pad(&ctx->pad_config);
                    update_interactivity_status(&ctx->pad_config, ctx->processes_count);
                }
            };
            break;
        case KEY_HOME:
            // remove highlighting
            remove_highlight(&ctx->pad_config);
            // next process's y
            ctx->pad_config.selected_process_y = 0;
            // get new process
            get_selected_process(
                &ctx->processes,
                &ctx->y_to_pid,
                &ctx->pad_config.selected_process,
                &ctx->pad_config.get_process_faild,
                ctx->pad_config.selected_process_y);
            // add highlighting if successful
            if (!ctx->pad_config.get_process_faild)
            {
                ctx->pad_config.y = ctx->pad_config.selected_process_y;
                highlight_process(&ctx->pad_config);
                refresh_pad(&ctx->pad_config);
                update_interactivity_status(&ctx->pad_config, ctx->processes_count);
            }
            break;
        case KEY_END:
            // remove highlighting
            remove_highlight(&ctx->pad_config);
            // next process's y
            ctx->pad_config.selected_process_y = ctx->processes_count - 1;
            // get new process
            get_selected_process(
                &ctx->processes,
                &ctx->y_to_pid,
                &ctx->pad_config.selected_process,
                &ctx->pad_config.get_process_faild,
                ctx->pad_config.selected_process_y);
            // highlight new process
            if (!ctx->pad_config.get_process_faild)
            {
                // only update padconfig's y on success
                ctx->pad_config.y = ctx->pad_config.selected_process_y;
                highlight_process(&ctx->pad_config);
                refresh_pad(&ctx->pad_config);
                update_interactivity_status(&ctx->pad_config, ctx->processes_count);
            }
            break;
        }

        pthread_mutex_unlock(&ctx->mutex);
        sleep(.4);
    }
cleanup:
    cleanup_context(ctx);
    // endwin();
    return NULL;
}