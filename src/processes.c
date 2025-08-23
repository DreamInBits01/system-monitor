#include "processes.h"
#include "utils.h"
struct dirent *get_processes()
{
    DIR *directory = opendir("/proc/");
    if (directory == NULL)
    {
        printf("Error while opening a directory!\n");
    };
    struct dirent *ep;
    struct dirent *processes = malloc(10 * sizeof(struct dirent));
    if (processes == NULL)
    {
        printf("Error while allocating processes\n");
        exit(1);
    }
    int processes_index = 0;
    while ((ep = readdir(directory)) != NULL && processes_index < 10)
    {
        if (is_numeric(ep->d_name))
        {
            processes[processes_index] = *ep;
            // printf("A process:%s\n", ep->d_name);
            processes_index++;
        }
    };
    closedir(directory);
    return processes;
}
