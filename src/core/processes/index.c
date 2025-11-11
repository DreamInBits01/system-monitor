#include "core/processes/index.h"

void read_processes_data(DIR *fd, ProcessesBlock *data)
{
    mark_processes_unseen(&data->processes);
    proc_dir_read_and_parse(
        fd,
        parse_processes_dir,
        data);
    remove_unseen_processes(&data->processes, &data->y_to_pid);
    data->processes_count = HASH_COUNT(data->processes);
}
void show_processes(ProcessesBlock *data)
{
    unsigned line_height = 0;
    Process *process = data->processes;
    mark_y_to_pid_unseen(&data->y_to_pid);
    while (line_height < data->virtual_pad.height && process != NULL)
    {
        // map y to process's pid in the y_to_pid
        YToPid *found_y_to_pid_entry;
        HASH_FIND_INT(data->y_to_pid, &line_height, found_y_to_pid_entry);
        if (found_y_to_pid_entry == NULL)
        {
            found_y_to_pid_entry = malloc(sizeof(YToPid));
            memset(found_y_to_pid_entry, 0, sizeof(YToPid));
            found_y_to_pid_entry->pid = process->pid;
            found_y_to_pid_entry->y = line_height;
            found_y_to_pid_entry->seen = true;
            HASH_ADD_INT(data->y_to_pid, y, found_y_to_pid_entry);
        }
        else
        {
            found_y_to_pid_entry->seen = true;
        }

        // render
        if (line_height == data->virtual_pad.y)
        {
            wattron(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            show_process_information(
                process,
                &data->window,
                data->virtual_pad.itself,
                line_height);
            wattroff(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        }
        else
        {
            show_process_information(
                process,
                &data->window,
                data->virtual_pad.itself,
                line_height);
        }
        // used when mapping from y to pid
        process->y = line_height;
        // get ready for the next operation
        process = process->hh.next;
        line_height++;
    }
    remove_y_to_pid_unseen_entries(&data->y_to_pid);
}
void cleanup_processes(Process **processes)
{
    Process *current_process, *tmp;
    HASH_ITER(hh, *processes, current_process, tmp)
    {
        HASH_DEL(*processes, current_process);
        cleanup_process(current_process);
    }
    *processes = NULL;
}
void cleanup_y_to_pid(YToPid **y_to_pid)
{
    YToPid *current_entry, *tmp;
    HASH_ITER(hh, *y_to_pid, current_entry, tmp)
    {
        HASH_DEL(*y_to_pid, current_entry);
        free(current_entry);
    }
    *y_to_pid = NULL;
}
void refresh_processes_pad(ProcessesBlock *data, unsigned processes_count)
{
    if (data->virtual_pad.y >= processes_count)
    {
        data->virtual_pad.y = processes_count - 1;
    }
    draw_processes_window(data);
    // prefresh(data->virtual_pad.itself,
    //          data->virtual_pad.y, data->virtual_pad.x + 2,
    //          data->window.y + 3,                       // move the pad 2 units down and show borders
    //          data->window.x + 2,                       // move the pad 2 units right and show borders
    //          data->window.y + data->window.height - 2, // move the pad 2 units up and show borders,
    //          data->window.x + data->window.width - 2   // move the pad 2 units left and show borders
    // );
    if (data->virtual_pad.y > data->virtual_pad.viewport_bottom)
    {
        data->virtual_pad.viewport_top += data->window.height - 4;
        data->virtual_pad.viewport_bottom += data->window.height - 4;
    }
    else if (data->virtual_pad.y < data->virtual_pad.viewport_top)
    {
        data->virtual_pad.viewport_top -= data->window.height - 4;
        data->virtual_pad.viewport_bottom -= data->window.height - 4;
    }
    prefresh(data->virtual_pad.itself,
             data->virtual_pad.viewport_top, data->virtual_pad.x + 2,
             data->window.y + 3,                       // move the pad 2 units down and show borders
             data->window.x + 2,                       // move the pad 2 units right and show borders
             data->window.y + data->window.height - 2, // move the pad 2 units up and show borders,
             data->window.x + data->window.width - 2   // move the pad 2 units left and show borders
    );
}
void handle_manual_process_selection(AppContext *ctx)
{
    // get new selected process
    get_selected_process(
        &ctx->processes_block->processes,
        &ctx->processes_block->y_to_pid,
        &ctx->processes_block->selected_process,
        &ctx->processes_block->get_process_faild,
        ctx->processes_block->selected_process_y);
    // add highlighting if successful
    if (!ctx->processes_block->get_process_faild)
    {
        ctx->processes_block->virtual_pad.y = ctx->processes_block->selected_process_y;
        highlight_process(ctx->processes_block);
        // refresh
        refresh_processes_pad(ctx->processes_block, ctx->processes_block->processes_count);

        // visible elements are in the range of pad view height
        // create a viewport_top (cursor position, starts at 0), viewport_bottom (viewtop+pad_view.height)
    }
}
/*

-if it doesn't exist, you can add it
-if it does, don't add it
-what if it doesn't exist in the new collection, but existes in the hashmap?
How to track dead processes?
1,2,3,4,5 | 1,2,4,5,6
Three exists | Three got deleted, six added.

Delta needs to be tracked


--Solution--
use a flag to indicate if a processes is seen or not,
the inital state should be not seen, and that should happen in each render cycle

then if the process exists, mark it as seen, if not, add it and mark it seen

finally, cleanup the processes that are still marked unseen, because these are delted
*/