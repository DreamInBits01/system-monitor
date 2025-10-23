#include "core/processes.h"
int is_numeric(char *name)
{
    while (*name)
    {
        if (!isdigit(*name))
            return 0;
        name++;
    };
    return 1;
}
void mark_processes_unseen(Process **processes)
{
    Process *process;
    for (process = *processes; process != NULL; process = process->hh.next)
    {
        process->seen = false;
    }
};
void remove_unseen_processes(Process **processes)
{
    Process *current, *tmp;
    HASH_ITER(hh, *processes, current, tmp)
    {
        if (current->seen == false)
        {
            HASH_DEL(*processes, current);
            free(current->exe_path);
            free(current);
        }
    }
};
void get_selected_process(Process **processes, YToPid **y_to_pid, Process **output, bool *get_process_faild, unsigned target_y)
{
    // map y to pid, then pid, to the corresponding process, then change its attributes shown on the screen
    YToPid *found_y_to_pid_entry;
    Process *previous_process = *output;
    HASH_FIND_INT(*y_to_pid, &target_y, found_y_to_pid_entry);
    if (found_y_to_pid_entry == NULL)
        *get_process_faild = true;
    ;
    HASH_FIND_INT(*processes, &found_y_to_pid_entry->pid, *output);
    if (*output == NULL)
    {
        // to not go out of boundry;
        *output = previous_process;
        *get_process_faild = true;
    }
    *get_process_faild = false;
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
void mark_y_to_pid_unseen(YToPid **y_to_pid)
{
    YToPid *y_to_pid_entry;
    for (y_to_pid_entry = *y_to_pid; y_to_pid_entry != NULL; y_to_pid_entry = y_to_pid_entry->hh.next)
    {
        y_to_pid_entry->seen = false;
    };
}
void remove_y_to_pid_unseen_entries(YToPid **y_to_pid)
{
    YToPid *current, *tmp;
    HASH_ITER(hh, *y_to_pid, current, tmp)
    {
        if (current->seen == false)
        {
            HASH_DEL(*y_to_pid, current);
            free(current);
        }
    }
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
void read_uptime(double *uptime, double *idle_time)
{
    FILE *proc_uptime = fopen("/proc/uptime", "r");
    if (!proc_uptime)
        return;
    fscanf(proc_uptime, "%lf %lf", uptime, idle_time);
    fclose(proc_uptime);
}
void read_process_cpu_usage(char *ep_name, Process *found_process)
{
    int pid_len = strlen(ep_name);
    char line[256];
    char *stat_path = malloc(7 + 6 + pid_len + 1);
    if (stat_path == NULL)
        return;
    sprintf(stat_path, "/proc/%s/stat", ep_name);
    FILE *process_stat = fopen(stat_path, "r");
    if (process_stat == NULL)
        return;

    if (fgets(line, sizeof(line), process_stat))
    {
        // the total time that the process has spent in the cpu, measured in seconds / the time the system has been running in seconds
        // on multi-core systems, this calculation gives the number of cores used, so it can be 1 core or 1.5 cores etc...
        // CPU Usage % = (CPU time delta / Wall time delta) × 100
        // to get the overall usage, you must divide the number of cores used by the system's total cores
        unsigned long utime, stime;
        char state;
        sscanf(line, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", &state, &utime, &stime);
        found_process->state = state;
        // calculate the cpu && wall time delta
        long tick_per_sec = sysconf(_SC_CLK_TCK);
        // divide by tick_per_sec to convert from clock tick unit to seconds
        double current_cpu_time = (double)(utime + stime) / tick_per_sec;
        double uptime, idle_time;
        read_uptime(&uptime, &idle_time);
        if (found_process->cpu_time)
        {
            // calc the delta
            double cpu_delta = current_cpu_time - found_process->cpu_time;
            double uptime_delta = uptime - found_process->last_uptime;
            if (uptime_delta > .5)
            {
                int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
                // divide by the total cores to get the overall percentage
                // if the process is using 1 core and the system has 10 cores, then the usage is 10%
                found_process->cpu_usage = cpu_delta / uptime_delta * 100 / num_cores;
            }
        }
        else
        {
            found_process->cpu_usage = 0;
        };
        // store the current raw values to be used next
        found_process->cpu_time = current_cpu_time;
        found_process->last_uptime = uptime;
    }

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
    int len = readlink(exe_file_name, exe_path, sizeof(exe_path) - 1);
    if (len != -1)
    {
        exe_path[len] = '\0';
        *destination = strdup(exe_path);
    }
    else
    {
        *destination = strdup("unknown");
    }
    free(exe_file_name);
}
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
                    printf("Error while allocating memory\n");
                }
                memset(found_process, 0, sizeof(Process));
                found_process->pid = pid;
                found_process->seen = true;
                found_process->type = ep->d_type;
                read_process_cpu_usage(ep->d_name, found_process);
                read_process_location(ep->d_name, &found_process->exe_path);
                if (strncmp(found_process->exe_path, "unknown", 7) != 0)
                {
                    HASH_ADD_INT(*processes, pid, found_process);
                }
            }
            else
            {
                found_process->seen = true;
                read_process_cpu_usage(ep->d_name, found_process);
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
            mvwprintw(pad, line_height, 0, "Process: %d, %s, cpu_usage:%.2f%%", process->pid, process->exe_path, process->cpu_usage);
            wattroff(pad, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        }
        else
        {
            mvwprintw(pad, line_height, 0, "Process: %d, %s, cpu_usage:%.2f%%", process->pid, process->exe_path, process->cpu_usage);
        }
        process->y = line_height;
        process = process->hh.next;
        line_height++;
    }
    remove_y_to_pid_unseen_entries(y_to_pid);
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