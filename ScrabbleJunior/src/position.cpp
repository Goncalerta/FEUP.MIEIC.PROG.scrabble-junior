#include <iostream> // TODO remove (as of now only used for debug purposes)
#include "position.h"

using namespace std;

Position::Position(): Position(0, 0) {}

Position::Position(int x, int y): x(x), y(y) {}

Position::Position(char x, char y) {
    // TODO check if characters are in limits 'A-Z', 'a-z'
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
}

void Position::stepForward(Orientation orientation) {
    if(orientation == Horizontal) {
        x += 1;
    } else {
        y += 1;
    }
}

void Position::stepBackwards(Orientation orientation) {
    if(orientation == Horizontal) {
        x -= 1;
    } else {
        y -= 1;
    }
}

pair<Position, Position> Position::laterals(Orientation orientation) {
    if(orientation == Horizontal) {
        return pair<Position, Position>(Position(x, y+1), Position(x, y-1));
    } else {
        return pair<Position, Position>(Position(x+1, y), Position(x-1, y));
    }
}
