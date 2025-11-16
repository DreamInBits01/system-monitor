#include "core/cpu/index.h"

void read_dynamic_cpu_data(FILE *fd, DynamicCpuData *data)
{
    proc_file_read_and_parse(
        fd,
        parse_dynamic_cpu_data,
        data);
}
void show_dynamic_cpu_data(CPUBlock *data)
{
    mvwprintw(data->window.itself, 4, 1, "Avg Mhz:%.2f", data->dynamic_data.avg_mhz);
}
void read_static_cpu_data(FILE *fd, StaticCpuData *data)
{
    data->logical_cpus = 0;
    proc_file_read_and_parse(
        fd,
        parse_static_cpu_data,
        data);
}
void show_static_cpu_data(CPUBlock *data)
{
    mvwprintw(data->window.itself, 1, 1, "Model name: %s", data->static_data.model_name);
    mvwprintw(data->window.itself, 2, 1, "Logical CPUs: %d", data->static_data.logical_cpus);
    mvwprintw(data->window.itself, 3, 1, "Pyhiscal cores: %d", data->static_data.physical_cores);
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
bool initialize_cpu_context(CPUBlock **data, int max_rows, int max_cols)
{
    *data = malloc(sizeof(CPUBlock));
    if (*data == NULL)
        return false;
    memset(*data, 0, sizeof(CPUBlock));
    (*data)->window.height = max_rows * .25;
    (*data)->window.width = max_cols * .48;
    (*data)->window.x = (max_cols) / 2;
    (*data)->window.y = 0;
    (*data)->window.itself = newwin(
        (*data)->window.height,
        (*data)->window.width,
        (*data)->window.y,
        (*data)->window.x);
    return true;
}
void cleanup_cpu_context(CPUBlock *data)
{
    delwin(data->window.itself);
    free(data);
}