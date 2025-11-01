#include "ui/interactivity.h"
/*
    TOOODOODOO
       - reflect the changes when a process gets killed,
        -to redraw the pad efficiently, just shift every y after the deleted process by -1

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
        // int ch = wgetch(ctx->pad_config.pad_view.itself);
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
            else
            {
                // YToPid *found_y_to_pid_entry;
                // HASH_FIND_INT(ctx->y_to_pid, ctx->pad_config.selected_process_y, found_y_to_pid_entry);
                // if (found_y_to_pid_entry != NULL)
                // {
                //     HASH_DEL(ctx->y_to_pid, found_y_to_pid_entry);
                //     free(found_y_to_pid_entry);
                // }
                // HASH_DEL(ctx->processes, ctx->pad_config.selected_process);
                // free(ctx->pad_config.selected_process);
                // read_processes(&ctx->processes, ctx->processes_count);
                // werase(ctx->pad_config.itself);
                // show_processes(&ctx->processes, &ctx->y_to_pid, ctx->pad_config.itself, ctx->pad_config.height, ctx->pad_config.y);
                // refresh_pad(&ctx->pad_config, ctx->processes_count);
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
                redraw_screen(ctx);
            }
            else
            {
                top_panel(ctx->sort_menu.panel);
                update_panels();
                doupdate();
            }
            ctx->sort_menu.visible = !ctx->sort_menu.visible;
            break;
        case KEY_RIGHT:
            if (ctx->pad_config.x < ctx->pad_config.width)
            {
                ctx->pad_config.x += 2;
                refresh_pad(&ctx->pad_config, ctx->processes_count);
            }
            break;
        case KEY_LEFT:
            if (ctx->pad_config.x > 0)
            {
                ctx->pad_config.x -= 2;
                refresh_pad(&ctx->pad_config, ctx->processes_count);
            }
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
        // usleep(50000);
        sleep(.4);
    }
cleanup:
    cleanup_context(ctx);
    // endwin();
    return NULL;
}