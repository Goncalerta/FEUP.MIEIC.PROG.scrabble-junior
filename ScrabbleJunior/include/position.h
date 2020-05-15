#ifndef POSITION_H
#define POSITION_H

#include <utility>
#include "orientation.h"

class Position {
    int x;
    int y;

    public:
    static bool isValid(char x, char y); // TODO should this belong to position?

    Position();
    Position(int x, int y);
    // TODO should this belong to position? maybe it could be a static method instead of constructor
    // somehow unified with `isValid` and removing the need for signed and unsigned constructors.
    Position(char x, char y); 

    int getX() const;
    int getY() const;

    void stepForward(Orientation orientation);
    void stepBackwards(Orientation orientation);
    std::pair<Position, Position> laterals(Orientation orientation) const;

    bool inLimits(int width, int height) const;

    bool operator==(const Position &other) const;
};

#endif