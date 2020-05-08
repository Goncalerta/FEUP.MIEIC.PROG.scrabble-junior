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

void Cell::cover() {
    covered = true;
    coverable = false;
}

std::pair<bool, bool> Cell::getPropagation() const {
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

bool Cell::canCover(const Hand &hand) const {
    if(!coverable) return false;
    return any_of(hand.begin(), hand.end(), [this](auto i){return i == this->letter;});
}
