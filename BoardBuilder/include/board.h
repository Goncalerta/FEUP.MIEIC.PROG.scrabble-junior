<<<<<<< HEAD
#pragma once
=======
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
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

<<<<<<< HEAD
public:
=======
    public:
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
    Board();
    Board(int width, int height);

    bool addWord(Word word);
    bool setWidth(int width);
    bool setHeight(int height);
<<<<<<< HEAD
    void printData(std::ostream& out) const;
    void printGrid(std::ostream& out) const;
=======
    void printData(std::ostream &out) const;
    void printGrid(std::ostream &out) const;
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
};

#endif