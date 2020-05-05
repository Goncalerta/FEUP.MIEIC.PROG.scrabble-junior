#include <algorithm>
#include "board.h"

using namespace std;

Board::Board(int width, int height): 
  width(width), 
  height(height), 
  total_letters(0),
  total_covered(0),
  grid(height, vector<Cell>(width)) 
{}

bool Board::addWord(Position pos, Orientation orientation, std::string word) {
    // TODO check if everything is valid
    getCell(pos).allowMove(orientation);
    for(int i = 0; i < word.size(); i++) {
        Cell &cell = getCell(pos);
        if(cell.isEmpty()) total_letters += 1;
        getCell(pos).setLetter(word[i]);
        pos.stepForward(orientation);
    }

    return true;
}

bool Board::setWidth(int width) {
    if(width > 0 && width <= 20) {
        this->width = width;
        for(auto &row: grid) {
            row.resize(width);
        }
        return true;
    } else {
        return false;
    }
}

bool Board::setHeight(int height) {
    if(height > 0 && height <= 20) {
        this->height = height;
        grid.resize(height, vector<Cell>(width));
        return true;
    } else {
        return false;
    }
}

bool Board::isFullyCovered() const {
    return total_covered == total_letters;
}

char Board::getLetter(Position position) const {
    return getCell(position).getLetter();
}

int Board::cover(Position position) {
    Cell &cell = getCell(position);

    cell.cover();
    pair<bool, bool> propagation = cell.getPropagation();
    bool propagates_horizontally = propagation.first;
    bool propagates_vertically = propagation.second;

    int score = 0;

    if(propagates_horizontally) {
        if(propagate(position, Horizontal)) score++;
    }

    if(propagates_vertically) {
        if(propagate(position, Vertical)) score++;
    }

    total_covered += 1;
    return score;
}

Cell& Board::getCell(Position position) {
    return grid[position.getY()][position.getX()];
}

const Cell& Board::getCell(Position position) const {
    return grid[position.getY()][position.getX()];
}

bool Board::propagate(Position pos, Orientation orientation) {
    Cell *cell;
    do {
        pos.stepForward(orientation);
        if(pos.getX() >= width || pos.getY() >= height) return true; 
        cell = &getCell(pos);
    } while(cell->isCovered());

    if(!cell->isEmpty()) {
        cell->allowMove(orientation);
    }

    return cell->isEmpty();
}

const Cell* Board::getNextUncoveredCell(Position pos, Orientation orientation) const {
    const Cell *cell;
    do {
        pos.stepForward(orientation);
        if(pos.getX() >= width || pos.getY() >= height) return nullptr; 
        cell = &getCell(pos);
    } while(cell->isCovered());

    if(cell->isEmpty()) {
        return nullptr;
    }

    return cell;
}

int Board::getHeight() const {
    return height;
}

int Board::getWidth() const {
    return width;
}

bool Board::hasMove(const char *hand_begin, const char *hand_end) {
    for(auto &row: grid) {
        for(auto &cell: row) {
            if(cell.canCover(hand_begin, hand_end)) return true;
        }
    }
    return false;
}

void Board::getLettersInBoard(vector<char> &letters) const {
    for(auto &row: grid) {
        for(auto &cell: row) {
            if(!cell.isEmpty()) letters.push_back(cell.getLetter());
        }
    }
}

bool Board::mustPlayTwiceEdgeCase(std::vector<Position> &positions, const char *hand_begin, const char *hand_end) {
    // This edge case happens when:
    // 1- All possible moves are with the same letter
    // 2- Player has just one such letter in hand
    // 3- At least one of the moves unlocks a cell that the player can cover
    // When all these conditions are met, only moves that respect '3' are legal
    // because moves that don't respect '3' would block the player from covering
    // a cell in the next move; however, rules say that players must always move
    // twice per turn whenever possible.
    char letter = 0;
    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            Cell &cell = grid[j][i];
            Position position(i, j);

            if(cell.canCover(hand_begin, hand_end)) {
                
                if(!letter) {
                    letter = cell.getLetter();
                    // Check condition '2'
                    if(count(hand_begin, hand_end, letter) == 2) return false;
                } else if(letter != cell.getLetter()) return false; // Check condition '1'

                // Check condition '3'
                pair<bool, bool> propagation = cell.getPropagation();
                bool propagates_horizontally = propagation.first;
                bool propagates_vertically = propagation.second;

                if(propagates_horizontally) {
                    const Cell *next_cell = getNextUncoveredCell(position, Horizontal);
                    if(next_cell) {
                        char next_letter = next_cell->getLetter();

                        if(next_letter != letter && any_of(hand_begin, hand_end, [next_letter](auto hand) { return next_letter == hand; })) {
                            positions.push_back(position);
                        }
                    }
                }

                if(propagates_vertically) {
                    const Cell *next_cell = getNextUncoveredCell(position, Vertical);
                    if(next_cell) {
                        char next_letter = next_cell->getLetter();

                        if(next_letter != letter && any_of(hand_begin, hand_end, [next_letter](auto hand) { return next_letter == hand; })) {
                            positions.push_back(position);
                        }
                    }
                }
            }
        }
    }

    return positions.size() >= 1;
}
