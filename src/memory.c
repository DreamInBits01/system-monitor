#include "memory.h"

void read_memory_info(MemoryInfo *memory_info)
{
    FILE *memory_info_file = fopen("/proc/meminfo", "r");
    if (memory_info_file == NULL)
    {
        printf("Error while reading memory_info_file\n");
        endwin();
        exit(1);
    }
    char line[256];
    // MemoryInfo memory_info = {0};
    while (fgets(line, sizeof(line), memory_info_file))
    {
        if (strncmp("MemTotal:", line, 9) == 0)
        {
            float kb;
            sscanf(line, "MemTotal: %f", &kb);
            memory_info->total = KB_TO_GB(kb);
        }
        if (strncmp("MemFree:", line, 8) == 0)
        {
            float kb;
            sscanf(line, "MemFree: %f", &kb);
            memory_info->free = KB_TO_GB(kb);
        }
        if (strncmp("Buffers:", line, 8) == 0)
        {
            unsigned kb;
            sscanf(line, "Buffers: %d", &kb);
            memory_info->buffers = kb;
        }
    };
}
void show_memory_info(MemoryInfo *memory_info, int bar_width)
{
    double used_percent = (memory_info->total - memory_info->free) / memory_info->total * 100;
    // get a fraction and multiply it by the width
    int memory_bar_fill = used_percent / 100 * bar_width;
    mvprintw(0, 0, "Memory total: %.2fgb", memory_info->total);
    mvprintw(1, 0, "Memory free: %.2fgb", memory_info->free);
    mvprintw(2, 0, "Buffers: %dkb", memory_info->buffers);
    mvprintw(3, 0, "Used memory:%.2f%%", used_percent);
    build_loadbar(memory_bar_fill, bar_width, 4, 0);
}