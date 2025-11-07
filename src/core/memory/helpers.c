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