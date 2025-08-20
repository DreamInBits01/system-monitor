#include "utils.h"
int is_numeric(char *name)
{
    while (*name)
    {
        if (!isdigit(*name))
            return 0;
        name++;
    };
    return 1;
}
void build_loadbar(int fill, int bar_width, int y, int x)
{
    mvaddch(y, x, '[');
    for (size_t i = 0; i < bar_width; i++)
    {
        if (i < fill)
        {
            if (i > fill - fill / 3)
            {
                attr_on(COLOR_PAIR(3), NULL); // red
                addch('|');
                attr_off(COLOR_PAIR(3), NULL);
            }
            else if (i > fill - fill / 2)
            {
                attr_on(COLOR_PAIR(2), NULL); // yellow
                addch('|');
                attr_off(COLOR_PAIR(2), NULL);
            }
            else
            {
                attr_on(COLOR_PAIR(1), NULL); // green
                addch('|');
                attr_off(COLOR_PAIR(1), NULL);
            }
        }
        else
        {
            addch(' ');
        };
    }
    addch(']');
}