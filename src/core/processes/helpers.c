#include "core/processes/helpers.h"
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

    HASH_FIND_INT(*processes, &found_y_to_pid_entry->pid, *output);
    if (*output == NULL)
    {
        // to not go out of boundry;
        *output = previous_process;
        *get_process_faild = true;
    }
    *get_process_faild = false;
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
    // memmove(comm, comm + 1, len - 2);
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
