#include "core/cpu/helpers.h"
void parse_cpuinfo_line(char *line, void *data)
{
    CPUData *cpu_data = (CPUData *)data;
    if (strncmp("cpu MHz", line, 7) == 0)
    {
        float current_mhz;
        if (sscanf(line, "cpu MHz : %f", &current_mhz) == 1)
        {
            cpu_data->total_mhz += current_mhz;
            cpu_data->mhz_occurrence += 1;
        }
    };
    if (cpu_data->mhz_occurrence > 0)
    {
        cpu_data->avg_mhz = cpu_data->total_mhz / cpu_data->mhz_occurrence;
    }
    if (strncmp("processor", line, 9) == 0)
    {
        cpu_data->logical_cpus++;
    }
    // model name
    if (strncmp("model name", line, 10) == 0 && strlen(cpu_data->model_name) == 0)
    {
        char *token = strtok(line + 13, " ");
        while (token != NULL)
        {
            if (strncmp(token, "Gen", 3) != 0 && strncmp(token, "Intel(R)", 8) != 0 && strncmp(token, "Core(TM)", 8) != 0)
            {
                strcat(cpu_data->model_name, token);
                strcat(cpu_data->model_name, " ");
            }
            token = strtok(NULL, " ");
        }
        //-1 for starting from zero and -1 for last added " "
        cpu_data->model_name[strlen(cpu_data->model_name) - 2] = '\0';
    }
    // pyhsical cores
    if (strncmp("cpu cores", line, 9) == 0 && cpu_data->physical_cores == 0)
    {
        cpu_data->physical_cores = atoi(line + 12);
    }
}
void parse_procstat_cpu_line(char *line, void *data)
{
    CPUData *cpu_data = (CPUData *)data;
    // Data
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    unsigned long long current_total_time;
    unsigned long long current_active_time;
    unsigned long long total_time_delta;
    unsigned long long active_time_delta;
    double usage;
    int index = -1;
    bool is_cpu_line = false;
    // Gather && process data
    if (strncmp("cpu", line, 3) == 0)
    {
        int parsed;
        if (line[3] == ' ')
        {
            parsed = sscanf(line + 3, "%llu %llu %llu %llu %llu %llu %llu",
                            &user,
                            &nice,
                            &system,
                            &idle,
                            &iowait,
                            &irq,
                            &softirq);
        }
        else
        {
            parsed = sscanf(line + 3, "%d %llu %llu %llu %llu %llu %llu %llu",
                            &index,
                            &user,
                            &nice,
                            &system,
                            &idle,
                            &iowait,
                            &irq,
                            &softirq);
        }
        if (parsed < 7 && index == -1)
            return;
        if (parsed < 8 && index > -1)
            return;
        if (parsed < 11)
        {
            steal = 0;
            guest = 0;
            guest_nice = 0;
        }
        // Calculate active & idle time
        current_total_time = user + nice + system + irq + softirq + idle + iowait + steal + guest + guest_nice;
        current_active_time = current_total_time - idle - iowait;
        is_cpu_line = true;
    }
    if (is_cpu_line && index >= 0 && index < cpu_data->cpu_cores_count)
    {
        // Calculate delta
        total_time_delta = current_total_time - cpu_data->cores[index].prev_total_time;
        active_time_delta = current_active_time - cpu_data->cores[index].prev_active_time;
        usage = (double)active_time_delta / (double)total_time_delta * 100;

        // Core is being processed
        if (total_time_delta > 50 && cpu_data->cores[index].prev_total_time > 0)
        {
            cpu_data->cores[index].usage = usage;
        }

        cpu_data->cores[index].prev_active_time = current_active_time;
        cpu_data->cores[index].prev_total_time = current_total_time;
    }
    else if (is_cpu_line && index == -1)
    {
        total_time_delta = current_total_time - cpu_data->prev_total_time;
        active_time_delta = current_active_time - cpu_data->prev_active_time;
        usage = (double)active_time_delta / (double)total_time_delta * 100;
        // Total cpu usage
        if (total_time_delta > 50 && cpu_data->prev_total_time > 0)
        {
            cpu_data->cpu_usage = usage;
        }

        cpu_data->prev_active_time = current_active_time;
        cpu_data->prev_total_time = current_total_time;
    }
}
void draw_cpu_window(CPUBlock *cpu_block)
{
    // Add box
    wattron(cpu_block->window.itself, COLOR_PAIR(4));
    box(cpu_block->window.itself, '|', 0);
    wattroff(cpu_block->window.itself, COLOR_PAIR(4));
    // Add metadata
    wattron(cpu_block->window.itself, A_BOLD);
    // CPU model
    mvwaddstr(cpu_block->window.itself, 0, 2, cpu_block->data.model_name);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M", cpu_block->data.system_stats.local_time);
    mvwprintw(cpu_block->window.itself, 0, (cpu_block->window.width - strlen(buffer)) / 2, "%s", buffer);
    unsigned days = cpu_block->data.system_stats.uptime_in_hours / 24;
    unsigned hours = cpu_block->data.system_stats.uptime_in_hours % 24;
    unsigned minutes = cpu_block->data.system_stats.uptime_in_minutes % 60;
    unsigned seconds = (unsigned)cpu_block->data.system_stats.uptime_in_seconds % 60;

    // Uptime
    mvwprintw(
        cpu_block->window.itself,
        cpu_block->window.height - 1,
        (cpu_block->window.width - 18 - count_digits(days) - count_digits(hours) - count_digits(minutes) - count_digits(seconds)) / 2,
        "Uptime: %hud, %hu:%hu:%hus", days, hours, minutes, seconds);

    // MhZ
    mvwprintw(cpu_block->window.itself, 0, cpu_block->window.width - 7 - count_digits(cpu_block->data.avg_mhz), "MhZ:%d", cpu_block->data.avg_mhz);
    wattroff(cpu_block->window.itself, A_BOLD);
    wrefresh(cpu_block->window.itself);
}
void parse_cpu_cores_count(char *line, void *output)
{
    size_t *count = (size_t *)output;
    if (strncmp(line, "cpu", 3) == 0)
        *count = *count + 1;
}
void cpu_usage_bar(WINDOW *window, float fill, int bar_width, int y, int x)
{
    mvwaddch(window, y, x, '[');
    int x_position = x;
    for (int i = 0; i < bar_width; i++)
    {
        x_position = x + i + 1;
        if (i <= fill)
        {
            if (i >= bar_width * .6)
            {
                wattron(window, COLOR_PAIR(3)); // red
                mvwaddch(window, y, x_position, '|');
                wattroff(window, COLOR_PAIR(3));
            }
            else if (i >= bar_width * .3)
            {
                wattron(window, COLOR_PAIR(2)); // yellow
                mvwaddch(window, y, x_position, '|');

                wattroff(window, COLOR_PAIR(2));
            }
            else
            {
                wattron(window, COLOR_PAIR(1)); // green
                mvwaddch(window, y, x_position, '|');
                wattroff(window, COLOR_PAIR(1));
            }
        }
        else
        {
            // empty
            mvwaddch(window, y, x_position, ' ');
        };
    }
    mvwaddch(window, y, x_position, ']');
}