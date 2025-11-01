#ifndef UI_STRUCTS
#define UI_STRUCTS
#include <ncurses.h>
// UI Block
typedef struct
{
    WINDOW *itself;
    unsigned height, width, x, y;
    bool hidden;
} Window;
typedef struct
{
    /*
        -This is where large content is put
        -It must have a regular window that acts as a viewing frame
    */
    WINDOW *itself;
    unsigned height, width, y, x;
} VirtualPad;
#endif