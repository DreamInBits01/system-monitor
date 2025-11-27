#include "core/cpu/index.h"
#define CPU_USAGE_BAR_WIDTH 10
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
    int cores_per_page = 6;
    int pages = (cpu_block->data.cpu_cores_count + 4) / cores_per_page;
    // int page = 0;
    /*
    sol 1
    -Loop over the number of pages
    -Then loop over the items in that page (keep j outside)

    sol 2
    -Breaking point
    -Current page * 5
    */
    for (int page = 0; page < pages; page++)
    {
        int y = 1;
        int start_core = page * cores_per_page;
        int end_core = start_core + cores_per_page;
        if (end_core > cpu_block->data.cpu_cores_count)
        {
            end_core = cpu_block->data.cpu_cores_count;
        }
        for (int core = start_core; core < end_core; core++)
        {
            float fill = ((float)cpu_block->data.cores[core].usage / 100.0f * CPU_USAGE_BAR_WIDTH);
            int start_x = page * (cpu_block->window.width / 5);
            mvwprintw(
                cpu_block->window.itself,
                y,
                2 + start_x,
                "C%d:",
                core);
            cpu_usage_bar(
                cpu_block->window.itself,
                fill,
                CPU_USAGE_BAR_WIDTH,
                y,
                2 + start_x + 4 + (digit_count(core) - 1));
            mvwprintw(
                cpu_block->window.itself,
                y,
                CPU_USAGE_BAR_WIDTH + 7 + start_x + (digit_count(core) - 1),
                "%.1f%%",
                cpu_block->data.cores[core].usage);
            y++;
        }
    }
    draw_cpu_window(cpu_block);
}
unsigned cpu_cores_count(FILE *fd)
{
    size_t output = 0;
    proc_file_read_and_parse(
        fd,
        parse_cpu_cores_count,
        &output, 1);
    return output;
}