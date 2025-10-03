#include "core/processes.h"
#include "utils.h"
void get_selected_process(Process **processes, pid_t *pid, int target_y)
{
    Process *current_process = *processes;
    while (current_process != NULL)
    {
        if (current_process->y == target_y)
        {
            *pid = current_process->pid;
        }
        current_process = current_process->hh.next;
    }
}
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
                // divide by the total cores to get the percentage overall
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
    size_t len = readlink(exe_file_name, exe_path, sizeof(exe_path) - 1);
    if (len != -1)
    {
        exe_path[len] = '\0';
        *destination = strdup(exe_path);
    }
    else
    {
        *destination = NULL;
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
        return;

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
                memset(found_process, 0, sizeof(*found_process));
                found_process->pid = pid;
                found_process->seen = true;
                found_process->name = strdup(ep->d_name);
                found_process->type = ep->d_type;
                read_process_cpu_usage(ep->d_name, found_process);
                read_process_location(ep->d_name, &found_process->exe_path);
                if (found_process->exe_path != NULL)
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
            wattron(pad, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
            wprintw(pad, "Process: %s, %s, cpu_usage:%.2f%%, line-h:%ld", process->name, process->exe_path, process->cpu_usage, line_height);
            wattroff(pad, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        }
        else
        {
            wprintw(pad, "Process: %s, %s, cpu_usage:%.2f%%", process->name, process->exe_path, process->cpu_usage);
        }
        wprintw(pad, "\n");
        process->y = line_height;
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