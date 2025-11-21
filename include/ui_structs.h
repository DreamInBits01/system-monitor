#ifndef UI_STRUCTS
#define UI_STRUCTS
#include <ncurses.h>
#include <panel.h>

// UI Block
typedef struct
{
    WINDOW *itself;
    int height, width, x, y;
    bool hidden;
} Window;
typedef struct
{
    /*
        -This is where large content is put
        -It must be grouped with a regular window that acts as a viewing frame
    */
    WINDOW *itself;
    unsigned height, width, y, x, viewport_top, viewport_bottom;
} VirtualPad;
typedef struct
{
    WINDOW *window;
    PANEL *panel;
    bool visible;
} SortMenu;

#endif