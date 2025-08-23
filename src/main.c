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
        double used_percent = (memory_info.total - memory_info.free) / memory_info.total * 100;
        // get a fraction and multiply it by the width
        int memory_bar_fill = used_percent / 100 * bar_width;
        mvprintw(0, 0, "Task manager (press q to quit)\n");
        mvprintw(1, 0, "Memory total: %f\n", memory_info.total);
        mvprintw(2, 0, "Memory free: %f\n", memory_info.free);
        mvprintw(3, 0, "Buffers: %d\n", memory_info.buffers);
        mvprintw(4, 0, "Used memory:%f", used_percent);
        build_loadbar(memory_bar_fill, bar_width, 5, 0);

        // cpu
        // int cpu_bar_fill =
        mvprintw(6, 0, "Model name: %s\n", cpu_info.model_name);
        mvprintw(7, 0, "Logical CPUs: %d\n", cpu_info.logical_cpus);
        mvprintw(8, 0, "Pyhiscal cores: %d\n", cpu_info.physical_cores);
        mvprintw(9, 0, "Mhz:%f", cpu_info.total_mhz / cpu_info.mhz_occurrence);
        // build_loadbar(fill, bar_width, 10, 0);
        processes = get_processes(&procsses_count);
        unsigned procsses_y = 10;
        unsigned processes_index = 0;
        while (processes_index < procsses_count)
        {
            mvprintw(procsses_y, 0, "Process name:%s", processes[processes_index].d_name);
            processes_index++;
            procsses_y++;
        }
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