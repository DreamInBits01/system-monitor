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
    noecho();              // character written will not be shown on the screen
    cbreak();              // Disables line buffering, making characters typed by the user immediately available to the program (no need to press enter)
    nodelay(stdscr, TRUE); // It makes getch non-blocking, it doesn't wait for the user to write anything to continue the execution of the program
    keypad(stdscr, TRUE);
    curs_set(0);
    slk_set(1, "Quit", 1);
    slk_set(2, "Kill", 1);
    slk_set(3, "Sort", 1);
    slk_refresh();
}
void cleanup_ncurses()
{
    clear();
    refresh();
    endwin();
    delwin(stdscr);
}