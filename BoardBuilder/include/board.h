#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "word.h"

class Board {
    int width;
    int height;
    std::vector<Word> words;
    // Auxiliary matrix to track character positions
    // It is either \0 (empty cell) or a character A-Z (character that occupies the cell). 
    std::vector<std::vector<char>> grid;

    public:
    Board();
    Board(int width, int height);

    bool addWord(Word word);
    bool setWidth(int width);
    bool setHeight(int height);

    void printData(std::ostream& out) const;
    void printGrid(std::ostream& out) const;
};

#endif