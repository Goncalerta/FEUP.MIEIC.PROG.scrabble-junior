#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "word.h"
#include "position.h"
#include "cell.h"

class Board {
    unsigned int width;
    unsigned int height;

    std::vector<std::vector<Cell>> grid;
    std::vector<Word> words;

    unsigned int total_letters;
    
    public:
    Board(unsigned int width, unsigned int height);

    unsigned int countLetters() const;
    unsigned int countWords() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    const Cell& getCell(Position position) const;

    bool isWordValid(const Word &word, std::ostream &error_messages) const;
    void addWord(Word &word);

    void loadWords(std::istream &save);
    void writeData(std::ostream &out) const;
};

#endif