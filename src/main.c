#include "main.h"

int main()
{
    slk_init(1);
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    noecho(); // wouldn't echo to the window
    cbreak(); // no need to press enter

    slk_set(1, "Help", 1);
    slk_set(2, "Save", 1);
    slk_set(3, "Load", 1);
    slk_set(4, "Quit", 1);
    slk_refresh();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    int bar_width = COLS / 4;
    MemoryInfo memory_info = {0};
    CpuInfo cpu_info = {0};
    struct dirent *processes;
    int procsses_count = 0;
    while (1)
    {
        memory_info = read_memory_info();
        cpu_info = read_cpu_info();
        mvprintw(0, 0, "Task manager (press q to quit)\n");
        show_memory_info(&memory_info, bar_width);
        show_cpu_info(&cpu_info);
        // build_loadbar(fill, bar_width, 10, 0);
        processes = get_processes(&procsses_count);
        unsigned procsses_y = 10;
        unsigned processes_index = 0;
        while (processes_index < procsses_count)
        {
            mvprintw(procsses_y, processes_index % 2 == 0 ? 0 : COLS / 8, "Process name:%s", processes[processes_index].d_name);
            processes_index++;
            if (processes_index % 2 == 0)
            {
                procsses_y++;
            }
        }
        free(processes);
        refresh();
        int ch = getch();
        if (ch == KEY_F(4))
        {
            break;
        }
        sleep(1.5);
    }
    endwin();
    // free(processes);
    return 0;
}