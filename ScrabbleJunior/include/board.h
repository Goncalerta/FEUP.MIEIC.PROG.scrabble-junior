#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "cell.h"
#include "position.h"
#include "orientation.h"
#include "word.h"
#include "hand.h"

class Board {
    unsigned int width;
    unsigned int height;

    unsigned int total_letters;
    unsigned int total_covered;
    std::vector<std::vector<Cell>> grid;

    Cell& getCell(Position position);
    
    bool propagate(Position pos, Orientation orientation);
    const Cell* getNextUncoveredCell(Position pos, Orientation orientation) const;

    public:
    Board();
    Board(unsigned int width, unsigned int height);
    Board& setSize(unsigned int width, unsigned int height);
    Board& setWidth(unsigned int width);
    Board& setHeight(unsigned int height);

    unsigned int countLetters() const;

    bool addWord(Word word);
    Word findWord(Position position, Orientation orientation);
    bool isFullyCovered() const;

    unsigned int getHeight() const;
    unsigned int getWidth() const;
    
    void cover(Position position, std::vector<Word> &completed_words);
    bool hasMove(const Hand &hand) const;

    const Cell& getCell(Position position) const;
    std::vector<char> getLettersInBoard() const;

    bool mustPlayTwiceEdgeCase(std::vector<Position> &positions, const Hand &hand);
};

#endif