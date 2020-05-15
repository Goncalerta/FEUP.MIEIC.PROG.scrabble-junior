#include "cell.h"

using namespace std;

Cell::Cell(char letter): 
    letter(letter)
{}

char Cell::getLetter() const {
    return letter;
}

Cell& Cell::setLetter(char l) {
    letter = l;
    return *this;
}

bool Cell::isEmpty() const {
    return letter == EMPTY;
}

ostream& operator<<(ostream &out, const Cell &cell) {
    return out << cell.letter;
}
