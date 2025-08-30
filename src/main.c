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
    Process *processes = NULL;
    Process *process;
    size_t procsses_count = 0;
    while (1)
    {
        memory_info = read_memory_info();
        cpu_info = read_cpu_info();
        show_memory_info(&memory_info, bar_width);
        show_cpu_info(&cpu_info);
        get_processes(&processes, &procsses_count);
        attron(A_BOLD);
        mvprintw(6, 0, "Processes count:%ld", procsses_count);
        attroff(A_BOLD);
        size_t procsses_y = 8;
        size_t index = 0;
        for (process = processes; process != NULL; process = process->hh.next)
        {
            mvprintw(procsses_y, index % 2 == 0 ? 0 : COLS / 4, "Process: %s, state:%c", process->name, process->state);
            if (index % 2 != 0)
            {
                procsses_y++;
            }
            index++;
        }
        refresh();
        int ch = getch();
        if (ch == KEY_F(4))
        {
            break;
        }
        sleep(2);
    }
    free(processes);
    endwin();
    return 0;
}