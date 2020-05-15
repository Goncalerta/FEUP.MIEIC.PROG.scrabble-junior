#ifndef POSITION_H
#define POSITION_H

#include <ostream>
#include "orientation.h"

class Position {
    friend std::ostream& operator<<(std::ostream &out, Position pos);

    int x;
    int y;

    public:
    Position();
    Position(int x, int y);
    // TODO should this belong to position? maybe it could be a static method instead of constructor
    // somehow unified with `isValid` and removing the need for signed and unsigned constructors.
    Position(char x, char y); 

    int getX() const;
    int getY() const;

    Position& stepForward(Orientation orientation);
    Position& stepBackwards(Orientation orientation);
    std::pair<Position, Position> laterals(Orientation orientation) const;

    bool inLimits(unsigned int width, unsigned int height) const;
};

std::ostream& operator<<(std::ostream &out, Position pos);

#endif