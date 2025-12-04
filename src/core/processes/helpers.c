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
// Parsing
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
                return;
            memset(found_process, 0, sizeof(Process));
            found_process->pid = pid;
            found_process->seen = true;
            found_process->type = ep->d_type;

            read_process_status(ep->d_name, found_process);
            read_process_cpu_usage(ep->d_name, found_process);
            HASH_ADD_INT(processes_data->processes, pid, found_process);
        }
        else
        {
            found_process->seen = true;
            read_process_cpu_usage(ep->d_name, found_process);
        }
    }
}
void parse_process_status_line(char *line, void *output)
{
    Process *process = (Process *)output;
    if (strncmp(line, "Name:", 5) == 0)
    {
        char buffer[17];
        buffer[0] = '\0';
        sscanf(line + 5, "%s", buffer);
        int buffer_len = strlen(buffer);
        if (buffer_len > 2 && buffer[0] == '(')
        {
            memmove(buffer, buffer + 1, buffer_len - 1);
        }
        if (buffer_len > 2 && buffer[buffer_len - 1] == ')')
        {
            memmove(buffer, buffer, buffer_len - 2);
        };
        char *tokenized_buffer = strtok(buffer, "/");
        process->name = strdup(tokenized_buffer);
    }
    if (strncmp(line, "State:", 6) == 0)
    {
        char state;
        sscanf(line + 6, " %c", &state);
        process->state = state;
    }
    if (strncmp(line, "Threads:", 8) == 0)
    {
        int threads;
        sscanf(line + 8, "%d", &threads);
        process->threads = threads;
    }
    if (strncmp(line, "Uid:", 4) == 0)
    {
        uid_t uid;
        sscanf(line + 4, "%u", &uid);
        struct passwd *owner = getpwuid(uid);
        process->owner = strdup(owner->pw_name);
    }
}
// Reading
void read_uptime(double *uptime, double *idle_time)
{
    FILE *proc_uptime = fopen("/proc/uptime", "r");
    if (!proc_uptime)
        return;
    fscanf(proc_uptime, "%lf %lf", uptime, idle_time);
    fclose(proc_uptime);
}
void read_process_cpu_usage(char *ep_name, Process *output)
{
    int ep_name_len = strlen(ep_name);
    char *stat_path = malloc(14 + ep_name_len + 1);
    if (stat_path == NULL)
        return;
    sprintf(stat_path, "/proc/%s/stat", ep_name);
    FILE *stat_fd = fopen(stat_path, "r");
    if (stat_fd == NULL)
    {
        free(stat_path);
        return;
    }
    char line[256];
    if (fgets(line, sizeof(line), stat_fd))
    {
        // the total time that the process has spent in the cpu, measured in seconds / the time the system has been running in seconds
        // on multi-core systems, this calculation gives the number of cores used, so it can be 1 core or 1.5 cores etc...
        // CPU Usage % = (CPU time delta / Wall time delta) × 100
        // to get the overall usage, you must divide the number of cores used by the system's total cores
        unsigned long utime, stime;
        int scanf_result = sscanf(line, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", &utime, &stime);
        if (scanf_result < 2)
        {
            utime = 0;
            stime = 0;
        }
        // calculate the cpu && wall time delta
        long tick_per_sec = sysconf(_SC_CLK_TCK);
        // divide by tick_per_sec to convert from clock tick unit to seconds
        double current_cpu_time = (double)(utime + stime) / tick_per_sec;
        double uptime, idle_time;
        read_uptime(&uptime, &idle_time);
        if (output->cpu_time)
        {
            // calc the delta
            double cpu_delta = current_cpu_time - output->cpu_time;
            double uptime_delta = uptime - output->last_uptime;
            if (uptime_delta > .5)
            {
                int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
                // divide by the total cores to get the overall percentage
                // if the process is using 1 core and the system has 10 cores, then the usage is 10%
                output->cpu_usage = cpu_delta / uptime_delta * 100 / num_cores;
            }
        }
        else
        {
            output->cpu_usage = 0;
        };
        // store the current raw values to be used next
        output->cpu_time = current_cpu_time;
        output->last_uptime = uptime;
    }
    free(stat_path);
    fclose(stat_fd);
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
    free(exe_path);
}
void read_process_status(char *ep_name, Process *process)
{
    int ep_name_len = strlen(ep_name);
    char *status_path = malloc(16 + ep_name_len + 1);
    if (status_path == NULL)
        return;
    sprintf(status_path, "/proc/%s/status", ep_name);
    FILE *process_status_file = fopen(status_path, "r");
    if (process_status_file == NULL)
        return;
    proc_file_read_and_parse(
        process_status_file,
        parse_process_status_line,
        process,
        0);
    // cleanup
    free(status_path);
    fclose(process_status_file);
}

// Processes state housekeeping
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
    Process *current_process, *tmp_process;
    HASH_ITER(hh, *processes, current_process, tmp_process)
    {
        if (current_process->seen == false)
        {
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
    {
        *get_process_faild = true;
        return;
    }

    HASH_FIND_INT(*processes, &found_y_to_pid_entry->pid, *output);
    if (*output == NULL)
    {
        *get_process_faild = true;
        return;
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

// UI
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
        "%d/%d", data->selected_process->pid,
        data->processes_count - 1);
    mvwprintw(
        data->window.itself,
        0,
        15,
        "%d", data->selected_process_y);
    mvwprintw(data->window.itself, 2, 2, "PID");
    mvwprintw(data->window.itself, 2, data->window.width * .12, "CPU");
    mvwprintw(data->window.itself, 2, data->window.width * .2, "State");
    mvwprintw(data->window.itself, 2, data->window.width * .3, "Threads");
    mvwprintw(data->window.itself, 2, data->window.width * .45, "User");
    mvwprintw(data->window.itself, 2, data->window.width * .62, "Name");
    // mvwprintw(data->window.itself, 2, data->window.width * .65, "Path");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
}
void show_process_information(Process *process, Window *window, WINDOW *virtual_pad, int y)
{
    if (process == NULL || process->owner == NULL || process->name == NULL)
        return;
    wattron(window->itself, COLOR_PAIR(5));
    if (process->is_deleted)
    {
    }
    mvwprintw(virtual_pad, y, 2, "%d", process->pid);
    mvwprintw(virtual_pad, y, (window->width * .12), "%.2f", process->cpu_usage);
    mvwprintw(virtual_pad, y, (window->width * .2), "%c", process->state);
    mvwprintw(virtual_pad, y, (window->width * .3), "%d", process->threads);
    mvwprintw(virtual_pad, y, (window->width * .45), "%s", process->owner);
    mvwprintw(virtual_pad, y, (window->width * .62), "%s", process->name);
    // mvwprintw(virtual_pad, y, (window->width * .65), "%s", process->exe_path);
    wattroff(window->itself, COLOR_PAIR(5));
}
void update_interactivity_metadata(ProcessesBlock *data, int processes_count)
{
    mvwprintw(data->window.itself, 1, 2, "Count:%d", processes_count);
    mvwprintw(data->window.itself, 1, 25, "Scrolled:%.0f%%", (float)data->virtual_pad.y / (processes_count - 1) * 100);
}
void highlight_process(ProcessesBlock *data)
{
    if (data->selected_process != NULL && !data->get_process_faild)
    {
        wattron(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        show_process_information(
            data->selected_process,
            &data->window,
            data->virtual_pad.itself,
            data->virtual_pad.y);
        wattroff(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
    }
}

// Cleanup
void cleanup_process(Process *process)
{
    free(process->exe_path);
    free(process->name);
    free(process->owner);
    free(process);
}