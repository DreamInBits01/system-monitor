#include "core/cpu/index.h"

void read_cpuinfo_data(FILE *fd, CPUData *data)
{
    data->logical_cpus = 0;

    proc_file_read_and_parse(
        fd,
        parse_cpuinfo_line,
        data,
        0);
}
void read_procstat_cpu_data(FILE *fd, CPUData *data)
{
    proc_file_read_and_parse(
        fd,
        parse_procstat_cpu_line,
        data,
        1);
}
void show_cpu_data(CPUBlock *cpu_block)
{
    int y = 1;
    for (int i = 0; i < cpu_block->data.cpu_cores_count; i++)
    {
        if (i < (cpu_block->data.cpu_cores_count / 2))
        {
            mvwprintw(cpu_block->window.itself, y, 2, "C%d:%.2f%%", i, cpu_block->data.cores[i].usage);
        }
        if (i == floor(((cpu_block->data.cpu_cores_count - 1) / 2)))
        {
            y = 0;
            mvwprintw(cpu_block->window.itself, y, cpu_block->window.width * .2, "C%d:%.2f%%", i, cpu_block->data.cores[i].usage);
        }
        else
        {
            mvwprintw(cpu_block->window.itself, y, cpu_block->window.width * .2, "C%d:%.2f%%", i, cpu_block->data.cores[i].usage);
        }
        y++;
    }

    // mvwprintw(cpu_block->window.itself, 1, 1, "Model name: %s", cpu_block->data.model_name);
    // mvwprintw(cpu_block->window.itself, 2, 1, "Logical CPUs: %d", cpu_block->data.logical_cpus);
    // mvwprintw(cpu_block->window.itself, 3, 1, "Pyhiscal cores: %d", cpu_block->data.physical_cores);
    // mvwprintw(cpu_block->window.itself, 4, 1, "Cores: %ld", cpu_block->data.cpu_cores_count);
    wattron(cpu_block->window.itself, COLOR_PAIR(4));
    box(cpu_block->window.itself, 0, 0);
    wattroff(cpu_block->window.itself, COLOR_PAIR(4));
    wattron(cpu_block->window.itself, A_BOLD);
    mvwaddstr(cpu_block->window.itself, 0, 2, cpu_block->data.model_name);
    mvwprintw(cpu_block->window.itself, 0, cpu_block->window.width - 10, "MhZ:%d", cpu_block->data.avg_mhz);
    wattroff(cpu_block->window.itself, A_BOLD);
    wrefresh(cpu_block->window.itself);
}
// void show_cpuinfo_data(CPUBlock *cpu_block)
// {
//     mvwprintw(cpu_block->window.itself, 1, 1, "Model name: %s", cpu_block->data.model_name);
//     mvwprintw(cpu_block->window.itself, 2, 1, "Logical CPUs: %d", cpu_block->data.logical_cpus);
//     mvwprintw(cpu_block->window.itself, 3, 1, "Pyhiscal cores: %d", cpu_block->data.physical_cores);
//     mvwprintw(cpu_block->window.itself, 4, 1, "Cores: %ld", cpu_block->data.cpu_cores_count);
//     mvwprintw(cpu_block->window.itself, 5, 1, "Avg Mhz:%.2f", cpu_block->data.avg_mhz);
//     wattron(cpu_block->window.itself, COLOR_PAIR(4));
//     box(cpu_block->window.itself, 0, 0);
//     wattroff(cpu_block->window.itself, COLOR_PAIR(4));
//     wattron(cpu_block->window.itself, A_BOLD);
//     mvwaddstr(cpu_block->window.itself, 0, 2, "CPU");
//     wattroff(cpu_block->window.itself, A_BOLD);
//     wrefresh(cpu_block->window.itself);
// }
size_t cpu_cores_count(FILE *fd)
{
    size_t output = -1;
    proc_file_read_and_parse(
        fd,
        parse_cpu_cores_count,
        &output, 1);
    return output;
}