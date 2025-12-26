#include "core/cpu/index.h"
#define CORE_USAGE_BAR_WIDTH 10
#define CPU_USAGE_BAR_WIDTH 20
void read_cpuinfo_data(FILE *fd, CPUData *data)
{
    data->logical_cpus = 0;
    data->mhz_occurrence = 0;
    data->total_mhz = 0;
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
        0);
}
void show_cpu_data(CPUBlock *cpu_block)
{
    int cores_per_page = 6;
    if (cores_per_page > (cpu_block->window.height - 2))
    {
        cores_per_page = cpu_block->window.height - 3;
    }
    int pages = (cpu_block->data.cpu_cores_count + 4) / cores_per_page;
    for (int page = 0; page < pages; page++)
    {
        int y = 1;
        int start_core = page * cores_per_page;
        int end_core = start_core + cores_per_page;
        int core_bar_width = cpu_block->window.width * .06;
        if (end_core > cpu_block->data.cpu_cores_count)
        {
            end_core = cpu_block->data.cpu_cores_count;
        }
        for (int core = start_core; core < end_core; core++)
        {
            float fill = ((float)cpu_block->data.cores[core].usage / 100.0f * core_bar_width);
            int start_x = page * (cpu_block->window.width / 5);
            mvwprintw(
                cpu_block->window.itself,
                y,
                2 + start_x,
                "C%d:",
                core);
            //+4 is to account for 'CPU:' so that the usage bar doesn't cover it
            cpu_usage_bar(
                cpu_block->window.itself,
                fill,
                core_bar_width,
                y,
                2 + start_x + 4 + (count_digits(core) - 1));
            mvwprintw(
                cpu_block->window.itself,
                y,
                2 + core_bar_width + 5 + start_x + (count_digits(core) - 1),
                "%.1f%%",
                cpu_block->data.cores[core].usage);
            y++;
        }
    }
    float cpu_usage_fill = ((float)cpu_block->data.cpu_usage / 100.0f * CPU_USAGE_BAR_WIDTH);
    mvwprintw(cpu_block->window.itself, cpu_block->window.height - 2, 2, "CPU:");
    cpu_usage_bar(
        cpu_block->window.itself,
        cpu_usage_fill,
        CPU_USAGE_BAR_WIDTH,
        cpu_block->window.height - 2,
        2 + 4);
    mvwprintw(
        cpu_block->window.itself,
        cpu_block->window.height - 2,
        2 + CPU_USAGE_BAR_WIDTH + 4 + 1,
        "%.1f%%", cpu_block->data.cpu_usage);
    draw_cpu_window(cpu_block);
}
int cpu_cores_count(FILE *fd)
{
    int output = 0;
    proc_file_read_and_parse(
        fd,
        parse_cpu_cores_count,
        &output, 1);
    return output;
}
void resize_cpu_block(Window *window, int max_rows, int max_cols)
{
    window->height = (int)(max_rows * .28);
    window->width = (int)max_cols;
    if (window->itself != NULL)
    {
        wresize(window->itself, window->height, window->width);
        mvwin(window->itself, window->y, window->x);
    }
    // window->itself = newwin(
    //     window->height,
    //     window->width,
    //     window->y,
    //     window->x);
}