#include "core/memory/index.h"

void read_memory_data(MemoryData *data)
{
    FILE *memory_data_file = fopen("/proc/meminfo", "r");
    if (memory_data_file == NULL)
    {
        printf("Error while reading memory_data_file\n");
        endwin();
        exit(1);
    }
    char line[256];
    while (fgets(line, sizeof(line), memory_data_file))
    {
        if (data->total == 0 && strncmp("MemTotal:", line, 9) == 0)
        {
            float kb;
            sscanf(line, "MemTotal: %f", &kb);
            data->total = KB_TO_GB(kb);
        }
        if (strncmp("MemFree:", line, 8) == 0)
        {
            float kb;
            sscanf(line, "MemFree: %f", &kb);
            data->free = KB_TO_GB(kb);
        }
        if (strncmp("Buffers:", line, 8) == 0)
        {
            unsigned kb;
            sscanf(line, "Buffers: %d", &kb);
            data->buffers = kb;
        }
    };
    fclose(memory_data_file);
}
void show_memory_data(MemoryBlock *data)
{
    double used_percent = (data->data.total - data->data.free) / data->data.total * 100;
    // get a fraction and multiply it by the width
    int memory_bar_fill = used_percent / 100 * data->bar_width;
    mvwprintw(data->window.itself, 1, 1, "Memory total: %.2fgb", data->data.total);
    mvwprintw(data->window.itself, 2, 1, "Memory free: %.2fgb", data->data.free);
    mvwprintw(data->window.itself, 3, 1, "Buffers: %dkb", data->data.buffers);
    mvwprintw(data->window.itself, 4, 1, "Used memory:%.2f%%", used_percent);
    build_loadbar(data->window.itself, memory_bar_fill, data->bar_width, 5, 1);
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, 0, 0);
    wattroff(data->window.itself, COLOR_PAIR(4));
    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "Memory");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
}