#include "processes.h"
#include "utils.h"
void cleanup_processes(Process **processes)
{
    Process *current, *tmp;
    HASH_ITER(hh, *processes, current, tmp)
    {
        HASH_DEL(*processes, current);
        free(current->name);
        free(current->exe_path);
        free(current);
    }
    free(processes);
}
void read_process_stat(char *ep_name, char *destination)
{
    int pid_len = strlen(ep_name);
    char line[256];
    char *stat_path = malloc(7 + 6 + pid_len + 1);
    if (stat_path == NULL)
    {
        printf("Error while alloacting file name\n");
        return;
    }
    sprintf(stat_path, "/proc/%s/stat", ep_name);
    FILE *process_stat = fopen(stat_path, "r");
    if (process_stat == NULL)
    {
        printf("Error while opening process's stat\n");
        return;
    }
    fgets(line, sizeof(line), process_stat);
    sscanf(line, "%*d %*s %c", destination);
    // char comm[17];
    // sscanf(line, "%*d %16s %c", comm, destination);
    // size_t len = strlen(comm);
    // if (len >= 2 && comm[0] == '(' && comm[len - 1] == ')')
    // {
    //     memmove(comm, comm + 1, len - 2);
    //     comm[len - 2] = '\0';
    // }
    // if (strncmp(comm, "index", 16) == 1)
    // {
    //     printf("Comm:%s\n", comm);
    // }
    fclose(process_stat);
    free(stat_path);
}
void read_process_location(char *ep_name, char **destination)
{
    int pid_len = strlen(ep_name);
    char exe_path[PROCESS_EXE_PATH_SIZE];
    char *exe_file_name = malloc(7 + pid_len + 5 + 1);
    sprintf(exe_file_name, "/proc/%s/exe", ep_name);
    size_t len = readlink(exe_file_name, exe_path, sizeof(exe_path) - 1);
    if (len != -1)
    {
        exe_path[len] = '\0';
        *destination = strdup(exe_path);
    }
    else
    {
        *destination = strdup("[unavailable]");
    }
    free(exe_file_name);
}
void mark_processes_unseen(Process **processes)
{
    Process *process;
    for (process = *processes; process != NULL; process = process->hh.next)
    {
        process->seen = false;
    }
};
void remove_unseen_processes(Process **processes, size_t *processes_count)
{
    Process *current, *tmp;
    HASH_ITER(hh, *processes, current, tmp)
    {
        if (current->seen == false)
        {
            HASH_DEL(*processes, current);
            free(current->exe_path);
            free(current->name);
            free(current);
        }
        else if (current->seen == true)
        {
            *processes_count += 1;
        }
    }
};
void read_processes(Process **processes, size_t *count)
{
    DIR *directory = opendir("/proc/");
    if (directory == NULL)
    {
        printf("Error while opening a directory!\n");
    };
    struct dirent *ep;
    size_t processes_count = 0;
    // mark all processes unseen
    mark_processes_unseen(processes);
    while ((ep = readdir(directory)) != NULL)
    {
        if (is_numeric(ep->d_name))
        {
            int pid = atoi(ep->d_name);
            Process *found_process = NULL;
            HASH_FIND_INT(*processes, &pid, found_process);
            if (found_process == NULL)
            {
                found_process = malloc(sizeof(Process));
                memset(found_process, 0, sizeof(*found_process));
                if (found_process == NULL)
                {
                    printf("Error while allocating memory\n");
                }
                found_process->pid = pid;
                found_process->seen = true;
                found_process->name = strdup(ep->d_name);
                found_process->type = ep->d_type;
                read_process_stat(ep->d_name, &found_process->state);
                read_process_location(ep->d_name, &found_process->exe_path);
                HASH_ADD_INT(*processes, pid, found_process);
            }
            else
            {
                found_process->seen = true;
            }
        }
    };
    remove_unseen_processes(processes, &processes_count);
    closedir(directory);
    *count = processes_count;
}
void show_processes(Process **processes, WINDOW *pad, int pad_height, int pad_y)
{
    size_t line_height = 0;
    Process *process = *processes;

    while (line_height < pad_height && process != NULL)
    {
        if (line_height == pad_y)
        {
            // wattron(pad, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            wprintw(pad, "Process: %s, %s", process->name, process->exe_path);
            // wattroff(pad, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        }
        else
        {
            wprintw(pad, "Process: %s, %s", process->name, process->exe_path);
        }
        wprintw(pad, "\n");
        line_height++;
        process = process->hh.next;
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