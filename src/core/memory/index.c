#include "core/memory/index.h"

void read_memory_data(FILE *fd, MemoryData *data)
{
    proc_file_read_and_parse(
        fd,
        parse_memory_line,
        data, 0);
}
void show_memory_data(MemoryBlock *data)
{
    double used_percent = (data->data.total - data->data.available) / data->data.total * 100;
    // get a fraction and multiply it by the width
    // int memory_bar_fill = (int)((float)used_percent / 100 * data->bar_width);
    mvwprintw(data->window.itself, 1, 2, "Total: %.2fgb", data->data.total);
    mvwprintw(data->window.itself, 1, data->window.width / 3, "Available: %.2fgb", data->data.available);
    mvwprintw(data->window.itself, 3, 2, "Free: %.2fgb", data->data.free);
    mvwprintw(data->window.itself, 3, data->window.width / 3, "Buffers: %lukb", data->data.buffers);
    mvwprintw(data->window.itself, 5, 2, "Used:%.2f%%", used_percent);
    // build_loadbar(data->window.itself, memory_bar_fill, data->bar_width, 4, data->window.width / 3);
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, '|', 0);
    wattroff(data->window.itself, COLOR_PAIR(4));
    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "Memory");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
}
void resize_memory_block(Window *window, int max_rows, int max_cols)
{
    window->y = (int)(max_rows * .28);
    window->x = (int)(max_cols * .55);
    window->height = (int)(.35 * max_rows);
    window->width = (int)(.46 * max_cols);
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