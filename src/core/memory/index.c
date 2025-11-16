#include "core/memory/index.h"

void read_memory_data(FILE *fd, MemoryData *data)
{
    proc_file_read_and_parse(
        fd,
        parse_memory_line,
        data);
}
void show_memory_data(MemoryBlock *data)
{
    double used_percent = (data->data.total - data->data.free) / data->data.total * 100;
    // get a fraction and multiply it by the width
    int memory_bar_fill = used_percent / 100 * data->bar_width;
    mvwprintw(data->window.itself, 1, 1, "Memory total: %.2fgb", data->data.total);
    mvwprintw(data->window.itself, 2, 1, "Memory free: %.2fgb", data->data.free);
    mvwprintw(data->window.itself, 3, 1, "Memory Available: %.2fgb", data->data.available);
    mvwprintw(data->window.itself, 4, 1, "Buffers: %lukb", data->data.buffers);
    mvwprintw(data->window.itself, 6, 1, "Used memory:%.2f%%", used_percent);
    build_loadbar(data->window.itself, memory_bar_fill, data->bar_width, 7, 1);
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, 0, 0);
    wattroff(data->window.itself, COLOR_PAIR(4));
    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "Memory");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
}
