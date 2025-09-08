#include "main.h"
// main thread for handling rendering
//  worker thread for handling interactivity
int main()
{
    slk_init(1);
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
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
    MemoryInfo memory_info = {0};
    CpuInfo cpu_info = {0};
    Process *processes = NULL;
    size_t processes_count = 0;

    // PAD Attributes (the container of the data)
    int processes_pad_height = 1000;
    int processes_pad_width = 200;
    int pad_y = 0, pad_x = 0;

    // WINDOW SIZE - What we can see on screen
    int processes_window_height = (int)(.7 * LINES);
    int processes_window_width = COLS;
    int processes_window_y = 8, processes_window_x = 0;

    WINDOW *pad = newpad(processes_pad_height, processes_pad_width);
    WINDOW *view_border = newwin(processes_window_height, processes_window_width, processes_window_y, processes_window_x);
    box(view_border, 0, 0);
    wrefresh(view_border);

    while (1)
    {
        memory_info = read_memory_info();
        cpu_info = read_cpu_info();
        show_memory_info(&memory_info, bar_width);
        show_cpu_info(&cpu_info);
        get_processes(&processes, &processes_count);

        attron(A_BOLD);
        mvprintw(6, 0, "Processes count:%ld", processes_count);
        mvprintw(6, 25, "Scrolled:%.1f%%", (float)pad_y / (processes_count - processes_window_height) * 100);
        attroff(A_BOLD);
        refresh();

        werase(pad);
        show_processes(&processes, pad, processes_pad_height, pad_y);
        prefresh(pad,
                 pad_y, pad_x,
                 processes_window_y, processes_window_x,
                 processes_window_y + processes_window_height - 1, processes_window_x + processes_window_width - 1);
        int ch = getch();
        switch (ch)
        {
        case KEY_F(4):
            goto cleanup;
        case KEY_UP:
            if (pad_y > 0)
                pad_y--;
            break;
        case KEY_DOWN:
            if (pad_y < processes_count - processes_window_height)
                pad_y++;
            break;
        case KEY_HOME:
            pad_y = 0;
            break;
        case KEY_END:
            pad_y = processes_pad_height - processes_window_height;
            break;
        }
        sleep(2);
    }

cleanup:
    delwin(pad);
    delwin(view_border);
    cleanup_processes(&processes);
    endwin();
    return 0;
}