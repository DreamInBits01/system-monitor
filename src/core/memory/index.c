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
void show_memory_data(MemoryBlock *memory_block)
{
    double used_percent = (memory_block->data->total - memory_block->data->free) / memory_block->data->total * 100;
    // get a fraction and multiply it by the width
    int memory_bar_fill = used_percent / 100 * memory_block->bar_width;
    mvwprintw(memory_block->window.itself, 1, 1, "Memory total: %.2fgb", memory_block->data->total);
    mvwprintw(memory_block->window.itself, 2, 1, "Memory free: %.2fgb", memory_block->data->free);
    mvwprintw(memory_block->window.itself, 3, 1, "Buffers: %dkb", memory_block->data->buffers);
    mvwprintw(memory_block->window.itself, 4, 1, "Used memory:%.2f%%", used_percent);
    build_loadbar(memory_block->window.itself, memory_bar_fill, memory_block->bar_width, 5, 1);
    box(memory_block->window.itself, 0, 0);
    wrefresh(memory_block->window.itself);
}