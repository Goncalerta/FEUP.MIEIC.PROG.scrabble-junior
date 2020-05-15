#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include "orientation.h"

class Position {
    friend std::ostream& operator<<(std::ostream &out, const Position &pos);

    int x;
    int y;

    public:
    Position();
    Position(int x, int y);
    Position(char x, char y); 

    int getX() const;
    int getY() const;

    Position& stepForward(Orientation orientation);
    Position& stepBackwards(Orientation orientation);
    std::pair<Position, Position> laterals(Orientation orientation) const;

    bool inLimits(int width, int height) const;
};

std::ostream& operator<<(std::ostream &out, const Position &pos);

#endif