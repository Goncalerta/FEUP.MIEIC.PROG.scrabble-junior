#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "cell.h"
#include "position.h"
#include "orientation.h"
#include "word.h"
#include "hand.h"

class Board {
    unsigned int width;
    unsigned int height;

    std::vector<std::vector<Cell>> grid;

    unsigned int total_letters;
    unsigned int total_covered;
    
    bool propagate(Position pos, Orientation orientation);
    const Cell* getNextUncoveredCell(Position pos, Orientation orientation) const;

    public:
    Board(unsigned int width, unsigned int height);
    void loadWords(std::istream &save);
    
    std::vector<char> getLettersInBoard() const;
    unsigned int countLetters() const;
    bool isFullyCovered() const;
    unsigned int getHeight() const;
    unsigned int getWidth() const;
    const Cell& getCell(Position position) const;

    void addWord(Word &word);
    Word findWord(Position position, Orientation orientation);
    
    void cover(Position position, std::vector<Word> &completed_words);
    bool hasMove(const Hand &hand) const;    

    bool mustPlayTwiceEdgeCase(std::vector<Position> &positions, const Hand &hand);
};

#endif