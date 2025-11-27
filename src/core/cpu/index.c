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

    // for (int i = 0; i < cpu_block->data.cpu_cores_count; i++)
    // {
    //     if (i == (current_page * maximum_pages))
    //     {
    //         mvwprintw(cpu_block->window.itself, 5, 40 + CPU_USAGE_BAR_WIDTH, "%d", current_page);
    //         current_page++;
    //     }
    //     if (i < (cpu_block->data.cpu_cores_count / 2))
    //     {
    //         mvwprintw(cpu_block->window.itself, y, 2, "C%d:", i);
    //         float fill = ((float)cpu_block->data.cores[i].usage / 100.0f * CPU_USAGE_BAR_WIDTH);
    //         cpu_usage_bar(cpu_block->window.itself, fill, CPU_USAGE_BAR_WIDTH, y, 4);
    //         mvwprintw(cpu_block->window.itself, y, CPU_USAGE_BAR_WIDTH + 5, "%.1f%%", cpu_block->data.cores[i].usage);
    //     }
    //     if (i == floor(((cpu_block->data.cpu_cores_count - 1) / 2)))
    //     {
    //         y = 0;
    //         mvwprintw(cpu_block->window.itself, y, cpu_block->window.width * .25, "C%d:", i);
    //         mvwprintw(cpu_block->window.itself, y, cpu_block->window.width * .25 + CPU_USAGE_BAR_WIDTH, "%.1f%%", cpu_block->data.cores[i].usage);
    //     }
    //     else
    //     {
    //         mvwprintw(cpu_block->window.itself, y, cpu_block->window.width * .2, "C%d:", i, cpu_block->data.cores[i].usage);
    //         mvwprintw(cpu_block->window.itself, y, cpu_block->window.width * .2 + CPU_USAGE_BAR_WIDTH, "%.1f%%", cpu_block->data.cores[i].usage);
    //     }
    //     y++;
    // }
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