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
            addch('#');
        }
        else
        {
            addch(' ');
        };
    }
    addch(']');
}