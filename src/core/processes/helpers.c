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
void cleanup_process(Process *process)
{
    free(process->exe_path);
    free(process->name);
    free(process);
}
void mark_processes_unseen(Process **processes)
{
    Process *process;
    for (process = *processes; process != NULL; process = process->hh.next)
    {
        process->seen = false;
    }
};
void remove_unseen_processes(Process **processes, YToPid **y_to_pid)
{
    Process *current_process, *tmp_process;
    YToPid *corresponding_y_to_pid;
    HASH_ITER(hh, *processes, current_process, tmp_process)
    {
        if (current_process->seen == false)
        {
            // need to delete the line height of the unseen process
            HASH_FIND_INT(*y_to_pid, &current_process->y, corresponding_y_to_pid);
            if (corresponding_y_to_pid != NULL)
            {
                HASH_DEL(*y_to_pid, corresponding_y_to_pid);
            }
            free(corresponding_y_to_pid);
            HASH_DEL(*processes, current_process);
            cleanup_process(current_process);
        }
    }
};
void get_selected_process(Process **processes, YToPid **y_to_pid, Process **output, bool *get_process_faild, unsigned target_y)
{
    // map y to pid, then pid, to the corresponding process, then change its attributes shown on the screen
    YToPid *found_y_to_pid_entry;
    HASH_FIND_INT(*y_to_pid, &target_y, found_y_to_pid_entry);
    if (found_y_to_pid_entry == NULL)
        *get_process_faild = true;

    HASH_FIND_INT(*processes, &found_y_to_pid_entry->pid, *output);
    if (*output == NULL)
    {
        *get_process_faild = true;
    }
    else
    {
        *get_process_faild = false;
    }
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
void read_process_cpu_usage(FILE *fd, Process *found_process)
{
    rewind(fd);
    char line[256];
    if (fgets(line, sizeof(line), fd))
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
            if (uptime_delta > .9)
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
}
void read_process_location(char *ep_name, char **destination)
{
    char *exe_path = malloc(12 + strlen(ep_name)); // "/proc/" + pid + "/exe" + '\0'
    if (exe_path == NULL)
        return;
    sprintf(exe_path, "/proc/%s/exe", ep_name);
    char buffer[PROCESS_EXE_PATH_SIZE];
    ssize_t len = readlink(exe_path, buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        // char terminated_buffer[33];
        // strcpy(terminated_buffer, buffer);
        // if (strlen(buffer) == 30)
        // {
        //     strcat(terminated_buffer, "...");
        // }
        // terminated_buffer[strlen(terminated_buffer)] = '\0';
        buffer[len] = '\0';
        *destination = strdup(buffer);
    }
    else
    {
        *destination = strdup("unknown");
    }
    // strcat()
    free(exe_path);
}
void read_process_name(FILE *fd, char **destination)
{
    rewind(fd);
    char line[256];
    char process_name[17];
    fgets(line, sizeof(line), fd);
    sscanf(line, "%*d %16s", process_name);
    int process_name_len = strlen(process_name);
    if (process_name_len >= 2 && process_name[0] == '(' && process_name[process_name_len - 1] == ')')
    {
        memmove(process_name, process_name + 1, process_name_len - 2);
        process_name[process_name_len - 2] = '\0';
    }
    else if (process_name_len >= 2 && process_name[0] == '(')
    {
        memmove(process_name, process_name + 1, process_name_len - 2);
        process_name[process_name_len - 2] = '\0';
    }
    // neglect anything after /
    char *tokenized_line = strtok(process_name, "/");
    *destination = strdup(tokenized_line);
}
void read_process_stat(char *ep_name, Process *process)
{
    int ep_name_len = strlen(ep_name);
    char *stat_path = malloc(14 + ep_name_len);
    if (stat_path == NULL)
        return;
    sprintf(stat_path, "/proc/%s/stat", ep_name);
    FILE *process_stat_file = fopen(stat_path, "r");
    if (process_stat_file == NULL)
        return;

    if (process->exe_path == NULL)
    {
        read_process_location(ep_name, &process->exe_path);
    }
    if (process->name == NULL)
    {
        read_process_name(process_stat_file, &process->name);
    }

    read_process_cpu_usage(process_stat_file, process);
    // reset
    free(stat_path);
    fclose(process_stat_file);
}
void parse_processes_dir(struct dirent *ep, void *data)
{
    ProcessesBlock *processes_data = (ProcessesBlock *)data;
    // check if the filename is numeric to determine if the file is a process
    if (is_numeric(ep->d_name))
    {
        int pid = atoi(ep->d_name);
        Process *found_process = NULL;
        HASH_FIND_INT(processes_data->processes, &pid, found_process);
        if (found_process == NULL)
        {
            found_process = malloc(sizeof(Process));
            if (found_process == NULL)
            {
                return;
            }
            memset(found_process, 0, sizeof(Process));
            found_process->pid = pid;
            found_process->seen = true;
            found_process->type = ep->d_type;

            read_process_stat(ep->d_name, found_process);
            HASH_ADD_INT(processes_data->processes, pid, found_process);
        }
        else
        {
            found_process->seen = true;
            read_process_stat(ep->d_name, found_process);
        }
    }
}
void draw_processes_window(ProcessesBlock *data)
{
    wclear(data->window.itself);
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, 0, 0);
    wattroff(data->window.itself, COLOR_PAIR(4));

    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "Processes");
    mvwprintw(
        data->window.itself,
        data->window.height - 1,
        2,
        "%d/%d", data->virtual_pad.y,
        data->processes_count - 1);
    mvwprintw(
        data->window.itself,
        0,
        15,
        "%d", data->selected_process->pid);
    mvwprintw(data->window.itself, 2, 2, "PID");
    mvwprintw(data->window.itself, 2, data->window.width * .16, "Name");
    mvwprintw(data->window.itself, 2, data->window.width * .5, "CPU");
    mvwprintw(data->window.itself, 2, data->window.width * .6, "Path");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
}
void show_process_information(Process *process, Window *window, WINDOW *virtual_pad, int y)
{
    if (process->exe_path == NULL || process->name == NULL)
        return;
    // reset what's shown on that location to avoid overlaping
    // this needs to be separated into a new function
    // mvwprintw(virtual_pad, y, 2, "%d", 0);
    // mvwprintw(virtual_pad, y, (window->width * .16), "%s", "\0");
    // mvwprintw(virtual_pad, y, (window->width * .5), "%.2f", 0);
    // mvwprintw(virtual_pad, y, (window->width * .6), "%s", "\0");

    mvwprintw(virtual_pad, y, 2, "%d", process->pid);
    mvwprintw(virtual_pad, y, (window->width * .16), "%s", process->name);
    mvwprintw(virtual_pad, y, (window->width * .5), "%.2f", process->cpu_usage);
    mvwprintw(virtual_pad, y, (window->width * .6), "%s", process->exe_path);
}
void update_interactivity_metadata(ProcessesBlock *data, int processes_count)
{
    mvwprintw(data->window.itself, 1, 2, "Count:%d", processes_count);
    mvwprintw(data->window.itself, 1, 25, "Scrolled:%.0f%%", (float)data->virtual_pad.y / (processes_count - 1) * 100);
}
void remove_process_highlight(ProcessesBlock *data)
{
    if (data->selected_process == NULL || data->get_process_faild)
        return;
    show_process_information(
        data->selected_process,
        &data->window,
        data->virtual_pad.itself,
        data->virtual_pad.y);
}
void highlight_process(ProcessesBlock *data)
{
    if (data->selected_process != NULL && !data->get_process_faild)
    {
        wattron(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        show_process_information(
            data->selected_process,
            &data->window, data->virtual_pad.itself,
            data->virtual_pad.y);
        wattroff(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
    }
}