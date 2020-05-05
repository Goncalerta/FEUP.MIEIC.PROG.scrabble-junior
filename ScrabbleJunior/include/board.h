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

    int total_letters;
    int total_covered;
    std::vector<std::vector<Cell>> grid;

    Cell& getCell(Position position);
    
    bool propagate(Position pos, Orientation orientation);
    const Cell* getNextUncoveredCell(Position pos, Orientation orientation) const;

    public:
    // TODO default maybe should be uninitialized
    Board(int width = 15, int height = 15);
    bool setWidth(int width);
    bool setHeight(int height);
    bool addWord(Position start, Orientation orientation, std::string word);
    bool isFullyCovered() const;

    int getHeight() const;
    int getWidth() const;
    
    char getLetter(Position position) const;
    int cover(Position position);
    bool hasMove(const char *hand_begin, const char *hand_end);

    const Cell& getCell(Position position) const;
    void getLettersInBoard(std::vector<char> &letters) const;

    bool mustPlayTwiceEdgeCase(std::vector<Position> &positions, const char *hand_begin, const char *hand_end);
};

#endif