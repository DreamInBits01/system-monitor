#include "ui/init.h"
void initialize_ncurses()
{
    // Ncurses config
    slk_init(1);
    initscr();
    start_color();
    // Colors
    init_color(44, 255, 255, 1);   // ORANGE
    init_color(55, 233, 227, 223); // OFFWHITE
    // PAIRS
    //  Status
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    // Borders and frames
    init_pair(4, COLOR_CYAN, COLOR_BLACK); // Orange
    init_pair(5, 55, COLOR_BLACK);         // OFFWHITE
    // Headers and titles
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);  // Section titles ("Memory total:", "Model name:")
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK); // Important labels

    noecho();              // character written will not be shown on the screen
    cbreak();              // Disables line buffering, making characters typed by the user immediately available to the program (no need to press enter)
    nodelay(stdscr, TRUE); // It makes getch non-blocking, it doesn't wait for the user to write anything to continue the execution of the program
    keypad(stdscr, TRUE);
    curs_set(0);
    slk_set(1, "Quit", 1);
    slk_set(2, "Kill", 1);
    slk_refresh();
    refresh();
}
void cleanup_ncurses()
{
    clear();
    refresh();
    endwin();
    delwin(stdscr);
}