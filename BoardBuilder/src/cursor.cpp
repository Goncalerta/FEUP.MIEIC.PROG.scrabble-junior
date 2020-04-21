#include "cursor.h"

Cursor::Cursor(int x, int y, Orientation orientation): Position(x, y), orientation(orientation) {}

Cursor::Cursor(Position pos, Orientation orientation): Position(pos), orientation(orientation) {}

void Cursor::stepForward() {
    if(orientation == Horizontal) { 
        x += 1;
    } else {
        y += 1;
    }
}

void Cursor::stepBackwards() {
    if(orientation == Horizontal) { 
        x -= 1;
    } else {
        y -= 1;
    }
}

void Cursor::stepLateral(int n) {
    if(orientation == Horizontal) { 
        y += n;
    } else {
        x += n;
    }
}

