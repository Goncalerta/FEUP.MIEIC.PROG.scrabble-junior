#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "position.h"
#include "orientation.h"

class Board {
    int width;
    int height;
    std::vector<std::vector<char>> grid;
    static const int EMPTY_CELL = '\0';

    public:
    Board();
    Board(int width, int height);
    bool setWidth(int width);
    bool setHeight(int height);
    bool addWord(Position start, Orientation orientation, std::string word);
    void printGrid(std::ostream &out) const;
};

#endif