#include "core/memory/helpers.h"
void build_loadbar(WINDOW *window, int fill, int bar_width, int y, int x)
{
    mvwaddch(window, y, x, '[');
    int x_position = x;
    for (int i = 0; i < bar_width; i++)
    {
        x_position = x + i + 1;
        if (i < fill)
        {
            if (i >= bar_width * .6)
            {
                wattron(window, COLOR_PAIR(3)); // red
                mvwaddch(window, y, x_position, '|');
                wattroff(window, COLOR_PAIR(3));
            }
            else if (i >= bar_width * .3)
            {
                wattron(window, COLOR_PAIR(2)); // yellow
                mvwaddch(window, y, x_position, '|');

                wattroff(window, COLOR_PAIR(2));
            }
            else
            {
                wattron(window, COLOR_PAIR(1)); // green
                mvwaddch(window, y, x_position, '|');
                wattroff(window, COLOR_PAIR(1));
            }
        }
        else
        {
            // empty
            mvwaddch(window, y, x_position, ' ');
        };
    }
    mvwaddch(window, y, x_position, ']');
}
void parse_memory_line(char *line, void *data)
{
    MemoryData *memory_data = (MemoryData *)data;
    if (memory_data->total == 0 && strncmp("MemTotal:", line, 9) == 0)
    {
        unsigned long kb;
        sscanf(line, "MemTotal: %lu", &kb);
        memory_data->total = (float)KB_TO_GB(kb);
    }
    if (strncmp("MemFree:", line, 8) == 0)
    {
        unsigned long kb;
        sscanf(line, "MemFree: %lu", &kb);
        memory_data->free = (float)KB_TO_GB(kb);
    }
    if (strncmp("MemAvailable:", line, 13) == 0)
    {
        unsigned long kb;
        sscanf(line, "MemAvailable: %lu", &kb);
        memory_data->available = (float)KB_TO_GB(kb);
    }
    if (strncmp("Buffers:", line, 8) == 0)
    {
        unsigned long kb;
        sscanf(line, "Buffers: %lu", &kb);
        memory_data->buffers = kb;
    }
}