#include <algorithm>
#include "board.h"

using namespace std;

Board::Board(): 
  total_letters(0),
  total_covered(0)
{}

Board::Board(unsigned int width, unsigned int height): 
  width(width), 
  height(height), 
  total_letters(0),
  total_covered(0),
  grid(height, vector<Cell>(width)) 
{}

bool Board::addWord(Word word) {
    // TODO check if everything is valid
    Position position = word.getStart();
    Orientation orientation = word.getOrientation();

    getCell(position).allowMove(orientation);
    for(const char &letter: word) {
        Cell &cell = getCell(position);
        if(cell.isEmpty()) total_letters += 1;
        cell.setLetter(letter);
        position.stepForward(orientation);
    }
    
    return true;
}

Word Board::findWord(Position position, Orientation orientation) {
    string word;
    
    while(position.inLimits(width, height) && !getCell(position).isEmpty()) {
        position.stepBackwards(orientation);
    }

    position.stepForward(orientation);
    Position start = position;

    while(position.inLimits(width, height) && !getCell(position).isEmpty()) {
        word.push_back(getCell(position).getLetter());
        position.stepForward(orientation);
    }

    return Word(start, orientation, word);
}

Board& Board::setSize(unsigned int width, unsigned int height) {
    return setWidth(width).setHeight(height);
}

Board& Board::setWidth(unsigned int width) {
    this->width = width;
    for(auto &row: grid) {
        row.resize(width);
    }
    
    return *this;
}

Board& Board::setHeight(unsigned int height) {
    this->height = height;
    grid.resize(height, vector<Cell>(width));
        
    return *this;
}

unsigned int Board::countLetters() const {
    return total_letters;
}

bool Board::isFullyCovered() const {
    return total_covered == total_letters;
}

char Board::getLetter(Position position) const {
    return getCell(position).getLetter();
}

void Board::cover(Position position, vector<Word> &completed_words) {
    Cell &cell = getCell(position);

    cell.cover();
    int score = 0;

    if(cell.propagatesHorizontally()) {
        if(propagate(position, Horizontal)) {
            completed_words.push_back(findWord(position, Horizontal));
        }
    }

    if(cell.propagatesVertically()) {
        if(propagate(position, Vertical)) {
            completed_words.push_back(findWord(position, Vertical));
        }
    }

    total_covered += 1;
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

bool Board::hasMove(const Hand &hand) const {
    for(auto &row: grid) {
        for(auto &cell: row) {
            char letter = cell.getLetter();
            if(cell.isCoverable() && hand.hasLetter(letter)) return true;
        }
    }
    return false;
}

vector<char> Board::getLettersInBoard() const {
    vector<char> letters;
    for(auto &row: grid) {
        for(auto &cell: row) {
            if(!cell.isEmpty()) letters.push_back(cell.getLetter());
        }
    }
    return letters;
}

bool Board::mustPlayTwiceEdgeCase(vector<Position> &positions, const Hand &hand) {
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

            if(cell.isCoverable() && hand.hasLetter(cell.getLetter())) {
                
                if(!letter) {
                    letter = cell.getLetter();
                    // Check condition '2'
                    if(count(hand.begin(), hand.end(), letter) == 2) return false;
                } else if(letter != cell.getLetter()) return false; // Check condition '1'

                // Check condition '3'
                if(cell.propagatesHorizontally()) {
                    const Cell *next_cell = getNextUncoveredCell(position, Horizontal);
                    if(next_cell) {
                        char next_letter = next_cell->getLetter();

                        if(next_letter != letter && any_of(hand.begin(), hand.end(), [next_letter](auto hand) { return next_letter == hand; })) {
                            positions.push_back(position);
                        }
                    }
                }

                if(cell.propagatesVertically()) {
                    const Cell *next_cell = getNextUncoveredCell(position, Vertical);
                    if(next_cell) {
                        char next_letter = next_cell->getLetter();

                        if(next_letter != letter && any_of(hand.begin(), hand.end(), [next_letter](auto hand) { return next_letter == hand; })) {
                            positions.push_back(position);
                        }
                    }
                }
            }
        }
    }

    return positions.size() >= 1;
}
