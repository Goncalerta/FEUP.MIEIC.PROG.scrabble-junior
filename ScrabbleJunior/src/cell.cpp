#include <algorithm>
#include "cell.h"

using namespace std;

Cell::Cell(char letter): 
    letter(letter), 
    covered(false),
    coverable(false),
    propagates_horizontally(false),
    propagates_vertically(false)
{}

void Cell::allowMove(Orientation orientation) {
    coverable = true;
    if(orientation == Horizontal) {
        propagates_horizontally = true;
    } else {
        propagates_vertically = true;
    }
}

pair<bool, bool> Cell::cover() {
    // if(!coverable) TODO
    covered = true;
    coverable = false;
    return pair<bool, bool>(propagates_horizontally, propagates_vertically);
}

char Cell::getLetter() const {
    return letter;
}

void Cell::setLetter(char l) {
    letter = l;
}

bool Cell::isCovered() const {
    return covered;
}

bool Cell::isCoverable() const {
    return coverable;
}

bool Cell::isEmpty() const {
    return letter == EMPTY;
}

bool Cell::canCover(const char *hand_begin, const char *hand_end) const {
    if(!coverable) return false;
    return any_of(hand_begin, hand_end, [this](auto i){return i == this->letter;});
}
