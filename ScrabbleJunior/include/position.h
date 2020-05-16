#ifndef POSITION_H
#define POSITION_H

#include <ostream>
#include <utility>
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

    bool operator==(const Position &other) const;
};

std::ostream& operator<<(std::ostream &out, const Position &pos);

#endif