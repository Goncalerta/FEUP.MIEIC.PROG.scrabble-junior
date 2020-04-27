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
    return pair(propagates_horizontally, propagates_vertically);
}

char Cell::getLetter() const {
    return letter;
}

char Cell::setLetter(char l) {
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
