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
        case 's':
        case 'S':
            ctx->processes_block->sort_option += 1;
            ctx->processes_block->sort_option = ctx->processes_block->sort_option % SORT_OPTS_COUNT;
            wattron(ctx->processes_block->window.itself, A_BOLD);
            mvwprintw(
                ctx->processes_block->window.itself,
                0,
                ctx->processes_block->window.width -
                    6 -
                    strlen(ctx->processes_block->sort_options[ctx->processes_block->sort_option]),
                "<S>:%s", ctx->processes_block->sort_options[ctx->processes_block->sort_option]);
            wattroff(ctx->processes_block->window.itself, A_BOLD);
            wrefresh(ctx->processes_block->window.itself);
            break;
        case KEY_F(1):
            goto cleanup;
            break;
        case KEY_F(2):
            if (ctx->processes_block->selected_process == NULL)
                return NULL;
            if (ctx->processes_block->selected_process->pid == 0 ||
                ctx->processes_block->selected_process->is_deleted)
                return NULL;
            int kill_result = kill(ctx->processes_block->selected_process->pid, SIGKILL);
            if (kill_result == -1)
            {
                perror("Kill faild!\n");
            }
            if (ctx->processes_block->selected_process->pid == getpid())
                goto cleanup;

            else
            {
                // YToPid *corresponding_y_to_pid_entry;
                // Mark it as deleted
                Process *found_process;
                HASH_FIND_INT(
                    ctx->processes_block->processes,
                    &ctx->processes_block->selected_process->pid,
                    found_process);
                if (found_process == NULL)
                    return NULL;
                found_process->is_deleted = true;
                // HASH_DEL(ctx->processes_block->y_to_pid, corresponding_y_to_pid_entry);
                // free(corresponding_y_to_pid_entry);
                // HASH_DEL(ctx->processes_block->processes, ctx->processes_block->selected_process);
                // free(ctx->processes_block->selected_process);
                // ctx->processes_block->selected_process = NULL;
                wattron(ctx->processes_block->virtual_pad.itself, COLOR_PAIR(3));
                mvwprintw(
                    ctx->processes_block->virtual_pad.itself,
                    ctx->processes_block->selected_process_y,
                    2,
                    "Deleted");
                // ctx->processes_block->selected_process_y -= 1;
                // handle_manual_process_selection(ctx->processes_block);
                wattroff(ctx->processes_block->virtual_pad.itself, COLOR_PAIR(3));
                // refresh_processes_pad(ctx->processes_block, ctx->processes_block->processes_count);
            }
            break;

        case KEY_F(3):
            if (ctx->sort_menu->visible)
            {
                hide_panel(ctx->sort_menu->panel);
                update_panels();
                doupdate();
                redraw_screen(ctx);
            }
            else
            {
                top_panel(ctx->sort_menu->panel);
                update_panels();
                doupdate();
            }
            ctx->sort_menu->visible = !ctx->sort_menu->visible;
            break;
        case KEY_UP:
            if (ctx->processes_block->virtual_pad.y > 0 && ctx->sort_menu->visible == false)
            {
                // remove highlighting
                remove_process_highlight(ctx->processes_block);
                // next process's y
                ctx->processes_block->selected_process_y = ctx->processes_block->virtual_pad.y - 1;
                handle_manual_process_selection(ctx->processes_block);
            }
            break;
        case KEY_DOWN:
            if (ctx->processes_block->virtual_pad.y < ctx->processes_block->processes_count - 1 && ctx->sort_menu->visible == false)
            {
                // remove highlighting
                remove_process_highlight(ctx->processes_block);
                // next process's y
                ctx->processes_block->selected_process_y = ctx->processes_block->virtual_pad.y + 1;
                handle_manual_process_selection(ctx->processes_block);
            };
            break;
        case KEY_HOME:
            // remove highlighting
            remove_process_highlight(ctx->processes_block);
            // next process's y
            ctx->processes_block->virtual_pad.viewport_top = 0;
            ctx->processes_block->virtual_pad.viewport_bottom = ctx->processes_block->window.height - INITIAL_VIEWPORT_BOTTOM_ALIGNMENT;
            ctx->processes_block->selected_process_y = 0;
            handle_manual_process_selection(ctx->processes_block);

            break;
        case KEY_END:
            // remove highlighting
            remove_process_highlight(ctx->processes_block);
            // next process's y
            // Because the pad is starting from y = 3, the viewport top must be the height of the window, with the added 3 and one to exclude last one viewed from the last viewport
            ctx->processes_block->virtual_pad.viewport_top = ctx->processes_block->processes_count - ctx->processes_block->window.height + PROCESS_PAD_ALIGNMENT;
            ctx->processes_block->virtual_pad.viewport_bottom = ctx->processes_block->processes_count - 1;
            // ctx->processes_block->window.y =
            ctx->processes_block->selected_process_y = ctx->processes_block->processes_count - 1;

            handle_manual_process_selection(ctx->processes_block);
            break;
        }
        // discard buffered keyboard strokes
        flushinp();
        pthread_mutex_unlock(&ctx->mutex);
        nanosleep(&ctx->interactivity_delay, NULL);
    }
cleanup:
    cleanup_context(ctx);
    cleanup_ncurses();
    exit(1);
}