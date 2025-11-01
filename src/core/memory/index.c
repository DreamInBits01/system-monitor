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
void show_memory_data(MemoryData *data, int bar_width)
{
    double used_percent = (data->total - data->free) / data->total * 100;
    // get a fraction and multiply it by the width
    int memory_bar_fill = used_percent / 100 * bar_width;
    mvprintw(0, 0, "Memory total: %.2fgb", data->total);
    mvprintw(1, 0, "Memory free: %.2fgb", data->free);
    mvprintw(2, 0, "Buffers: %dkb", data->buffers);
    mvprintw(3, 0, "Used memory:%.2f%%", used_percent);
    build_loadbar(memory_bar_fill, bar_width, 4, 0);
}