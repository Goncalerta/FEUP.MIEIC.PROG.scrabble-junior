#include "board.h"

using namespace std;

Board::Board(unsigned int width, unsigned int height): 
  width(width), 
  height(height), 
  grid(height, vector<Cell>(width)),
  total_letters(0)
{}

unsigned int Board::countLetters() const {
    return total_letters;
}

unsigned int Board::countWords() const {
    return words.size();
}

unsigned int Board::getHeight() const {
    return height;
}

unsigned int Board::getWidth() const {
    return width;
}

const Cell& Board::getCell(Position position) const {
    return grid[position.getY()][position.getX()];
}

Cell& Board::getCell(Position position) {
    return grid[position.getY()][position.getX()];
}

void Board::addWord(Word word) {
    // Position position = word.getStart();
    // Orientation orientation = word.getOrientation();

    // position.stepBackwards(orientation);
    // if(!position.inLimits(width, height) && !getCell(position).isEmpty()) {
    //     return false;
    // }

    // for(char current_letter: word) {
    //     position.stepForward(orientation);
    //     if(!position.inLimits(width, height)) {
    //         return false;
    //     }
    //     Cell &current_cell = getCell(position);

    //     if(current_cell.isEmpty()) {
    //         std::pair<Position, Position> laterals = position.laterals(orientation);

    //         if(laterals.first.inLimits(width, height) && !getCell(laterals.first).isEmpty()) {
    //             return false;
    //         }
    //         if(laterals.second.inLimits(width, height) && !getCell(laterals.second).isEmpty()) {
    //             return false;
    //         }
    //     } else if(current_cell.getLetter() == current_letter) {
    //         return false;
    //     }
    // }
    // position.stepForward(orientation);
    // if(!position.inLimits(width, height) && !getCell(position).isEmpty()) {
    //     return false;
    // }
    Position position = word.getStart();
    Orientation orientation = word.getOrientation();

    position = word.getStart();
    for(char letter: word) {
        getCell(position).setLetter(letter);
        position.stepForward(orientation);
    }

    words.push_back(word);
}

void Board::writeData(ostream &out) const {
    out << height << " x " << width << endl;

    for(const Word &word: words) {
        out << word << endl;
    }
}
