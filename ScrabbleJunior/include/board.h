#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "cell.h"
#include "position.h"
#include "orientation.h"

class Board {
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;

    Cell cellAtPosition(Position position) const;
    bool propagate(Position pos, Orientation orientation);

    public:
    static const int ILLEGAL_MOVE = -1;

    Board(int width = 15, int height = 15);
    bool setWidth(int width);
    bool setHeight(int height);
    bool addWord(Position start, Orientation orientation, std::string word);
    void printGrid(std::ostream &out) const;
    
    char getLetter(Position position) const;
    int cover(Position position); // returns score (0, 1 or 2), ILLEGAL_MOVE
};

#endif