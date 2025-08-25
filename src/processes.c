#include "processes.h"
#include "utils.h"
struct dirent *get_processes(struct dirent *processes, int *count)
{
    DIR *directory = opendir("/proc/");
    if (directory == NULL)
    {
        printf("Error while opening a directory!\n");
    };
    struct dirent *ep;
    int processes_capacity = INITIAL_PROCESSES_CAPACITY;
    // struct dirent *processes = malloc(processes_capacity * sizeof(struct dirent));
    if (processes == NULL)
    {
        printf("Error while allocating processes\n");
        exit(1);
    }
    unsigned processes_index = 0;

    /*
        --loop over the entries
        --check if it's added
        --if not malloc it, and add it
        --if it's already there,
    */

    // while ((ep = readdir(directory)) != NULL)
    // {
    //     if (processes_index >= processes_capacity)
    //     {
    //         processes_capacity *= INITIAL_PROCESSES_CAPACITY;
    //         struct dirent *temp = realloc(processes, processes_capacity * sizeof(struct dirent));
    //         if (temp == NULL)
    //         {
    //             printf("Re-allocation of processes failed\n");
    //             free(processes);
    //             exit(1);
    //         };
    //         processes = temp;
    //     }
    //     if (is_numeric(ep->d_name))
    //     {
    //         processes[processes_index] = *ep;
    //         processes_index++;
    //     }
    // };
    closedir(directory);
    *count = processes_index;
    return processes;
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