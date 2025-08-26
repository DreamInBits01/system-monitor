#include "processes.h"
#include "utils.h"
void mark_processes_unseen(Process **processes) {

};
void remove_unseen_processes(Process **processes) {

};
void get_processes(Process **processes, size_t *count)
{
    DIR *directory = opendir("/proc/");
    if (directory == NULL)
    {
        printf("Error while opening a directory!\n");
    };
    struct dirent *ep;
    size_t processes_index = 0;
    /*
        --loop over the entries
        --check if it's added
        --if not malloc it, and add it
        --if it's already there,
    */

    // mark all processes unseen
    while ((ep = readdir(directory)) != NULL)
    {
        if (is_numeric(ep->d_name))
        {
            int pid = atoi(ep->d_name);
            Process *found_process = NULL;
            HASH_FIND_INT(*processes, &pid, found_process);
            if (found_process == NULL)
            {
                int pid = atoi(ep->d_name);
                found_process = malloc(sizeof(Process));
                if (found_process == NULL)
                {
                    printf("Error while allocating memory\n");
                }
                found_process->id = pid;
                found_process->seen = true;
                found_process->name = strdup(ep->d_name);
                found_process->type = ep->d_type;
                HASH_ADD_INT(*processes, id, found_process);
                processes_index++;
            }
            else
            {
                // mark the process as seen
            }
        }
    };
    // clean up unseen processes
    closedir(directory);
    *count = processes_index;
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