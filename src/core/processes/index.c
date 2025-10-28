#include "core/processes/index.h"
void read_processes(Process **processes, unsigned *count)
{
    DIR *directory = opendir("/proc/");
    if (directory == NULL)
        return;

    struct dirent *ep;
    // mark all processes unseen
    mark_processes_unseen(processes);
    while ((ep = readdir(directory)) != NULL)
    {
        // check if the filename is numeric to determine if the file is a process
        if (is_numeric(ep->d_name))
        {
            int pid = atoi(ep->d_name);
            Process *found_process = NULL;
            HASH_FIND_INT(*processes, &pid, found_process);
            if (found_process == NULL)
            {
                found_process = malloc(sizeof(Process));
                if (found_process == NULL)
                {
                    closedir(directory);
                    return;
                }
                memset(found_process, 0, sizeof(Process));
                found_process->pid = pid;
                found_process->seen = true;
                found_process->type = ep->d_type;

                read_process_stat(ep->d_name, found_process);
                if (found_process->exe_path && strncmp(found_process->exe_path, "unknown", 7) != 0)
                {
                    HASH_ADD_INT(*processes, pid, found_process);
                }
                else
                {
                    // cleanup allocated memory
                    free(found_process->exe_path);
                    free(found_process);
                }
            }
            else
            {
                found_process->seen = true;
                read_process_stat(ep->d_name, found_process);
            }
        }
    };
    remove_unseen_processes(processes);
    *count = HASH_COUNT(*processes);
    closedir(directory);
}
void show_processes(Process **processes, YToPid **y_to_pid, WINDOW *pad, unsigned pad_height, unsigned pad_y)
{
    unsigned line_height = 0;
    Process *process = *processes;
    mark_y_to_pid_unseen(y_to_pid);
    while (line_height < pad_height && process != NULL)
    {
        // map y to process's pid in the y_to_pid
        YToPid *found_y_to_pid_entry;
        HASH_FIND_INT(*y_to_pid, &line_height, found_y_to_pid_entry);
        if (found_y_to_pid_entry == NULL)
        {
            found_y_to_pid_entry = malloc(sizeof(YToPid));
            memset(found_y_to_pid_entry, 0, sizeof(YToPid));
            found_y_to_pid_entry->pid = process->pid;
            found_y_to_pid_entry->y = line_height;
            found_y_to_pid_entry->seen = true;
            HASH_ADD_INT(*y_to_pid, y, found_y_to_pid_entry);
        }
        else
        {
            found_y_to_pid_entry->seen = true;
        }

        // render
        if (line_height == pad_y)
        {
            wattron(pad, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            show_process_information(process, pad, line_height);
            // mvwprintw(pad, line_height, 0, "Process: %d, %s, cpu_usage:%.2f%%", process->pid, process->exe_path, process->cpu_usage);
            wattroff(pad, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        }
        else
        {
            show_process_information(process, pad, line_height);

            // mvwprintw(pad, line_height, 0, "Process: %d, %s, cpu_usage:%.2f%%", process->pid, process->exe_path, process->cpu_usage);
        }
        process->y = line_height;
        process = process->hh.next;
        line_height++;
    }
    remove_y_to_pid_unseen_entries(y_to_pid);
}
void cleanup_processes(Process **processes)
{
    Process *current_process, *tmp;
    HASH_ITER(hh, *processes, current_process, tmp)
    {
        HASH_DEL(*processes, current_process);
        free(current_process->exe_path);
        free(current_process);
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