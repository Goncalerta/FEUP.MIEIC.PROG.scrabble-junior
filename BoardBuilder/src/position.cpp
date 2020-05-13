#include "position.h"

using namespace std;

Position::Position(): Position(0, 0) {}

Position::Position(int x, int y): x(x), y(y) {}

Position::Position(unsigned int x, unsigned int y): x(x), y(y) {}

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

void Position::stepForward(Orientation orientation) {
    if(orientation == Horizontal) x += 1;
    else y += 1;
}

void Position::stepBackwards(Orientation orientation) {
    if(orientation == Horizontal) x -= 1;
    else y -= 1;
}

pair<Position, Position> Position::laterals(Orientation orientation) const {
    if(orientation == Horizontal) {
        return pair<Position, Position>(Position(x, y+1), Position(x, y-1));
    } else {
        return pair<Position, Position>(Position(x+1, y), Position(x-1, y));
    }
}

bool Position::inLimits(int width, int height) const {
    return x < width && y < height;
}

ostream& operator<<(ostream &out, Position pos) {
    out << (pos.y + 'A') << (pos.x + 'a');
    return out;
}
