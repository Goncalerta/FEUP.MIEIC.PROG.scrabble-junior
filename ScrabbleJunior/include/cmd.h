#ifndef CMD_H
#define CMD_H

// Adapted from the file supplied by the teacher on Moodle.
// Provides an easy way of controlling the console colors and cursor, and to clear the screen.

#define NOMINMAX
#include <windows.h>

enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6, 
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

void clrscr();

// Position the cursor at column 'x', line 'y'
// The coodinates of upper left corner of the screen are (x,y) = (0,0)
void gotoxy(int x, int y);

// Set text color & background
void setcolor(Color color, Color background_color = BLACK);

#endif