// #include "ncurses.h"
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int buffers;
    float total;
    float free;
} MemInfo;

MemInfo read_memory()
{
    FILE *memory_file = fopen("/proc/meminfo", "r");
    if (memory_file == NULL)
    {
        printf("Error while opening the memory info file\n");
        exit(1);
    };
    char buffer[256];
    MemInfo info = {0};
    while (fgets(buffer, sizeof(buffer), memory_file))
    {
        if (strncmp("MemTotal:", buffer, 9) == 0)
        {
            float kb = 0;
            sscanf(buffer, "MemTotal: %f", &kb);
            info.total = kb / (1024 * 1024);
        }
        if (strncmp("MemFree:", buffer, 8) == 0)
        {
            float kb = 0;
            sscanf(buffer, "MemFree: %f", &kb);
            info.free = kb / (1024 * 1024);
        }
        if (strncmp("Buffers:", buffer, 8) == 0)
        {
            int kb = 0;
            sscanf(buffer, "Buffers: %d", &kb);
            info.buffers = kb;
        }
    }
    return info;
}
void build_loadbar(int fill, int bar_width, int y, int x)
{
    mvaddch(y, x, '[');
    for (size_t i = 0; i < bar_width; i++)
    {
        if (i < fill)
        {
            addch('#');
        }
        else
        {
            addch(' ');
        };
    }
    addch(']');
}

int main()
{
    slk_init(1);
    initscr();
    noecho();
    cbreak();

    slk_set(1, "Help", 1);
    slk_set(2, "Save", 1);
    slk_set(3, "Load", 1);
    slk_set(4, "Quit", 1);
    slk_refresh();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    int bar_width = COLS / 4;
    MemInfo memory_info = read_memory();
    while (1)
    {
        memory_info = read_memory();
        double used_percent = (memory_info.total - memory_info.free) / memory_info.total * 100;
        // get a fraction and multiply it by the width
        int fill = used_percent / 100 * bar_width;
        mvprintw(0, 0, "Task manager (press q to quit)\n");
        mvprintw(1, 0, "Memory total: %f\n", memory_info.total);
        mvprintw(2, 0, "Memory free: %f\n", memory_info.free);
        mvprintw(3, 0, "Buffers: %d\n", memory_info.buffers);
        mvprintw(4, 0, "Used memory:%f", used_percent);
        build_loadbar(fill, bar_width, 5, 0);
        refresh();
        int ch = getch();
        if (ch == KEY_F(4))
        {
            break;
        }
        sleep(1.5);
    }
    endwin();
    return 0;
}