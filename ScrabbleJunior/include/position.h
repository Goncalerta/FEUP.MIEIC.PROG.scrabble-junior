#ifndef POSITION_H
#define POSITION_H

#include <utility>
#include "orientation.h"

class Position {
    int x;
    int y;

    public:
    Position();
    Position(int x, int y);
    Position(char x, char y);

    int getX() const;
    int getY() const;
    char getXChar() const;
    char getYChar() const;

    void stepForward(Orientation orientation);
    void stepBackwards(Orientation orientation);
    std::pair<Position, Position> laterals(Orientation orientation);

    bool inRect(Position start, int width, int height) const;
};

#endif