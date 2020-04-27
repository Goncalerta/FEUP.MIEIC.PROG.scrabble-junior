#include "cursor.h"

<<<<<<< HEAD
Cursor::Cursor(int x, int y, Orientation orientation) : Position(x, y), orientation(orientation) {}

Cursor::Cursor(Position pos, Orientation orientation) : Position(pos), orientation(orientation) {}

void Cursor::stepForward() {
    if (orientation == Horizontal) {
        x += 1;
    }
    else {
=======
Cursor::Cursor(int x, int y, Orientation orientation): Position(x, y), orientation(orientation) {}

Cursor::Cursor(Position pos, Orientation orientation): Position(pos), orientation(orientation) {}

void Cursor::stepForward() {
    if(orientation == Horizontal) { 
        x += 1;
    } else {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        y += 1;
    }
}

void Cursor::stepBackwards() {
<<<<<<< HEAD
    if (orientation == Horizontal) {
        x -= 1;
    }
    else {
=======
    if(orientation == Horizontal) { 
        x -= 1;
    } else {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        y -= 1;
    }
}

void Cursor::stepLateral(int n) {
<<<<<<< HEAD
    if (orientation == Horizontal) {
        y += n;
    }
    else {
        x += n;
    }
}
=======
    if(orientation == Horizontal) { 
        y += n;
    } else {
        x += n;
    }
}

>>>>>>> af2634ed998871d93345b34770acad66809fd03f
