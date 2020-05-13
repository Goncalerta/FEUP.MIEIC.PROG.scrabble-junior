#ifndef BOARD_H
#define BOARD_H

#include <ostream>
#include <vector>
#include "word.h"
#include "position.h"
#include "cell.h"

class Board {
    unsigned int width;
    unsigned int height;

    unsigned int total_letters;
    std::vector<Word> words;

    // Auxiliary matrix to track character positions
    std::vector<std::vector<Cell>> grid;

    public:
    Board(unsigned int width, unsigned int height);

    unsigned int countLetters() const;
    unsigned int countWords() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    const Cell& getCell(Position position) const;
    Cell& getCell(Position position);

    void addWord(Word word);
    void writeData(std::ostream &out) const;
};

#endif