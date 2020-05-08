#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>
#include "position.h"
#include "orientation.h"
#include "hand.h"

class Cell {
    char letter;
    bool covered;
    bool coverable;
    bool propagates_horizontally;
    bool propagates_vertically;

    static const char EMPTY = '\0';
    
    public:
    Cell(char letter = EMPTY);
    void allowMove(Orientation word_orientation);
    void cover();
    std::pair<bool, bool> getPropagation() const;
    
    char getLetter() const;
    void setLetter(char l);

    bool isCovered() const;
    bool isCoverable() const;
    bool isEmpty() const;

    bool canCover(const Hand &hand) const;
};

#endif