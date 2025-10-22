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
                remove_process_highlight(&ctx->pad_config);
                // next process's y

                ctx->pad_config.selected_process_y = ctx->pad_config.y - 1;
                handle_manual_process_selection(ctx);
            }
            break;
        case KEY_DOWN:
            if (ctx->pad_config.y < ctx->processes_count - 1 && ctx->sort_menu.visible == false)
            {
                // remove highlighting
                remove_process_highlight(&ctx->pad_config);
                // next process's y
                ctx->pad_config.selected_process_y = ctx->pad_config.y + 1;
                handle_manual_process_selection(ctx);
            };
            break;
        case KEY_HOME:
            // remove highlighting
            remove_process_highlight(&ctx->pad_config);
            // next process's y
            ctx->pad_config.selected_process_y = 0;
            handle_manual_process_selection(ctx);

            break;
        case KEY_END:
            // remove highlighting
            remove_process_highlight(&ctx->pad_config);
            // next process's y
            ctx->pad_config.selected_process_y = ctx->processes_count - 1;
            handle_manual_process_selection(ctx);
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