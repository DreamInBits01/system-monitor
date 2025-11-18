#include "core/cpu/index.h"

void read_dynamic_cpu_data(FILE *fd, DynamicCpuData *data)
{
    proc_file_read_and_parse(
        fd,
        parse_dynamic_cpu_data,
        data, 0);
}
void show_dynamic_cpu_data(CPUBlock *data)
{
    mvwprintw(data->window.itself, 5, 1, "Avg Mhz:%.2f", data->dynamic_data.avg_mhz);
}
void read_static_cpu_data(FILE *fd, StaticCpuData *data)
{
    data->logical_cpus = 0;
    proc_file_read_and_parse(
        fd,
        parse_static_cpu_data,
        data,
        0);
}
void show_static_cpu_data(CPUBlock *data)
{
    mvwprintw(data->window.itself, 1, 1, "Model name: %s", data->static_data.model_name);
    mvwprintw(data->window.itself, 2, 1, "Logical CPUs: %d", data->static_data.logical_cpus);
    mvwprintw(data->window.itself, 3, 1, "Pyhiscal cores: %d", data->static_data.physical_cores);
    mvwprintw(data->window.itself, 4, 1, "Cores: %ld", data->static_data.cpu_cores_count);
}
void update_cpu_block(CPUBlock *data)
{
    show_static_cpu_data(data);
    show_dynamic_cpu_data(data);
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, 0, 0);
    wattroff(data->window.itself, COLOR_PAIR(4));
    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "CPU");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
}
size_t cpu_cores_count(FILE *fd)
{
    size_t output = -1;
    proc_file_read_and_parse(
        fd,
        parse_cpu_cores_count,
        &output, 1);
    return output;
}