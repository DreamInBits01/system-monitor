#include "ui/init.h"
void initialize_ncurses()
{
    // Ncurses config
    slk_init(1);
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    noecho();
    cbreak();
    curs_set(0);
    slk_set(1, "Quit", 1);
    slk_set(2, "Kill", 1);
    slk_refresh();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}