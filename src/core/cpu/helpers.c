#include "core/cpu/helpers.h"
// void parse_cpu_data(char *line, void *data) {

// };
void parse_cpuinfo_line(char *line, void *data)
{
    CPUData *cpu_data = (CPUData *)data;
    float total_mhz = 0;
    int mhz_occurrence = 0;
    if (strncmp("cpu MHz", line, 7) == 0)
    {
        float current_mhz;
        if (sscanf(line, "cpu MHz : %f", &current_mhz) == 1)
        {
            total_mhz += current_mhz;
            mhz_occurrence += 1;
        }
    };
    if (mhz_occurrence > 0)
    {
        float avg_mhz = (float)total_mhz / mhz_occurrence;
        cpu_data->avg_mhz = avg_mhz;
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
    if (strncmp("cpu", line, 3) == 0)
    {
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
        unsigned index;
        int parsed = sscanf(line, "cpu%u %llu %llu %llu %llu %llu %llu %llu",
                            &index,
                            &user,
                            &nice,
                            &system,
                            &idle,
                            &iowait,
                            &irq,
                            &softirq);
        if (parsed < 8)
            return;
        if (parsed < 11)
        {
            steal = 0;
            guest = 0;
            guest_nice = 0;
        }
        // Calculate active & idle time
        unsigned long long current_total_time = user + nice + system + irq + softirq + idle + iowait + steal + guest + guest_nice;
        unsigned long long current_active_time = current_total_time - idle - iowait;
        // Calculate delta
        unsigned long long total_time_delta = current_total_time - cpu_data->cores[index].prev_total_time;
        unsigned long long active_time_delta = current_active_time - cpu_data->cores[index].prev_active_time;
        if (total_time_delta > 50 && cpu_data->cores[index].prev_total_time > 0)
        {
            double cpu_usage = (double)active_time_delta / (double)total_time_delta * 100;
            cpu_data->cores[index].usage = cpu_usage;
        }

        cpu_data->cores[index].prev_active_time = current_active_time;
        cpu_data->cores[index].prev_total_time = current_total_time;
    }
}
void draw_cpu_window(CPUBlock *cpu_block)
{
    // Add box
    wattron(cpu_block->window.itself, COLOR_PAIR(4));
    box(cpu_block->window.itself, 0, 0);
    wattroff(cpu_block->window.itself, COLOR_PAIR(4));
    // Add metadata
    wattron(cpu_block->window.itself, A_BOLD);
    // CPU model
    mvwaddstr(cpu_block->window.itself, 0, 2, cpu_block->data.model_name);
    time_t now;
    time(&now);
    struct tm *local_time = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M", local_time);
    // Time
    mvwprintw(cpu_block->window.itself, 0, (cpu_block->window.width - strlen(buffer)) / 2, "%s", buffer);
    // MhZ
    mvwprintw(cpu_block->window.itself, 0, cpu_block->window.width - 10, "MhZ:%d", cpu_block->data.avg_mhz);
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
            // else
            // {
            //     wattron(window, COLOR_PAIR(1)); // green
            //     mvwaddch(window, y, x_position, '|');
            //     wattroff(window, COLOR_PAIR(1));
            // }
            else if (fill > 0)
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
void show_cpu_core_information(CPUBlock *cpu_block, int i, int y, int x)
{
    mvwprintw(cpu_block->window.itself, y, 2, "C%d:", i);
    int fill = (int)((float)cpu_block->data.cores[i].usage / 100.0f * CPU_USAGE_BAR_WIDTH);
    cpu_usage_bar(cpu_block->window.itself, fill, CPU_USAGE_BAR_WIDTH, y, 4);
    mvwprintw(cpu_block->window.itself, y, CPU_USAGE_BAR_WIDTH + 5, "%d%%", cpu_block->data.cores[i].usage);
}