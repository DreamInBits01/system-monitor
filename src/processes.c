#include "processes.h"
#include "utils.h"
struct dirent *get_processes(int *count)
{
    DIR *directory = opendir("/proc/");
    if (directory == NULL)
    {
        printf("Error while opening a directory!\n");
    };
    struct dirent *ep;
    int processes_capacity = INITIAL_PROCESSES_CAPACITY;
    struct dirent *processes = malloc(processes_capacity * sizeof(struct dirent));
    if (processes == NULL)
    {
        printf("Error while allocating processes\n");
        exit(1);
    }
    unsigned processes_index = 0;
    while ((ep = readdir(directory)) != NULL)
    {
        if (processes_index >= processes_capacity)
        {
            processes_capacity *= INITIAL_PROCESSES_CAPACITY;
            struct dirent *temp = realloc(processes, processes_capacity * sizeof(struct dirent));
            if (temp == NULL)
            {
                printf("Re-allocation of processes failed\n");
                free(temp);
                free(processes);
                exit(1);
            };
            processes = temp;
        }
        if (is_numeric(ep->d_name))
        {
            processes[processes_index] = *ep;
            processes_index++;
        }
    };
    closedir(directory);
    *count = processes_index;
    return processes;
}
