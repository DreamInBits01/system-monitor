#include "processes.h"
#include "utils.h"
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
    Process *process, *tmp;
    HASH_ITER(hh, *processes, process, tmp)
    {
        if (process->seen == false)
        {
            HASH_DEL(*processes, process);
            free(process);
        }
        else if (process->seen == true)
        {
            *processes_count += 1;
        }
    }
};
void get_processes(Process **processes, size_t *count)
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
                if (found_process == NULL)
                {
                    printf("Error while allocating memory\n");
                }
                found_process->pid = pid;
                found_process->seen = true;
                found_process->name = strdup(ep->d_name);
                found_process->type = ep->d_type;
                char line[256];
                int pid_len = strlen(ep->d_name);
                char *file_name = malloc(7 + 6 + pid_len + 1);
                if (file_name == NULL)
                {
                    printf("Error while alloacting file name\n");
                }
                sprintf(file_name, "/proc/%s/stat", ep->d_name);
                FILE *process_stat = fopen(file_name, "r");
                if (process_stat == NULL)
                {
                    printf("Error while opening process's stat\n");
                }
                fgets(line, sizeof(line), process_stat);
                sscanf(line, "%*d %*s %c", &found_process->state);
                fclose(process_stat);
                free(file_name);
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