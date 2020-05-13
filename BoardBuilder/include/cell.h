#ifndef CELL_H
#define CELL_H

#include <ostream>

class Cell {
    friend std::ostream& operator<<(std::ostream &out, const Cell &cell);

    static const char EMPTY = ' ';

    char letter;
    
    public:
    Cell(char letter = EMPTY);

    bool isEmpty() const;
    char getLetter() const;
    Cell& setLetter(char l);
};

std::ostream& operator<<(std::ostream &out, const Cell &cell);

#endif