#include "position.h"

<<<<<<< HEAD
Position::Position() : Position(0, 0) {}

Position::Position(int x, int y) : x(x), y(y) {}
=======
Position::Position(): Position(0, 0) {}

Position::Position(int x, int y): x(x), y(y) {}
>>>>>>> af2634ed998871d93345b34770acad66809fd03f

Position::Position(char x, char y) {
    this->x = x - 'a';
    this->y = y - 'A';
}

int Position::getX() const {
    return x;
}
int Position::getY() const {
    return y;
}

char Position::getXChar() const {
    return x + 'a';
}

char Position::getYChar() const {
    return y + 'A';
}

bool Position::inRect(Position start, int width, int height) const {
    return x >= start.x && x < start.x + width && y >= start.y && y < start.y + height;
<<<<<<< HEAD
}
=======
}
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
