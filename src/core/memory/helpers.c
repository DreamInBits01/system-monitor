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
                wattr_on(window, COLOR_PAIR(3), NULL); // red
                mvwaddch(window, y, x_position, '|');
                wattr_off(window, COLOR_PAIR(3), NULL);
            }
            else if (i >= bar_width * .3)
            {
                wattr_on(window, COLOR_PAIR(2), NULL); // yellow
                mvwaddch(window, y, x_position, '|');

                wattr_off(window, COLOR_PAIR(2), NULL);
            }
            else
            {
                wattr_on(window, COLOR_PAIR(1), NULL); // green
                mvwaddch(window, y, x_position, '|');
                wattr_off(window, COLOR_PAIR(1), NULL);
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