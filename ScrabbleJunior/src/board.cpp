#include <iostream> // TODO remove (as of now only used for debug purposes)
#include "board.h"

using namespace std;

Board::Board(int width, int height): 
  width(width), 
  height(height), 
  grid(height, vector<Cell>(width)) 
{}

bool Board::addWord(Position pos, Orientation orientation, std::string word) {
    // TODO check if everything is valid
    getCell(pos).allowMove(orientation);
    for(int i = 0; i < word.size(); i++) {
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

char Board::getLetter(Position position) const {
    return getCell(position).getLetter();
}

int Board::cover(Position position) {
    Cell &cell = getCell(position);
    if(!cell.isCoverable()) return ILLEGAL_MOVE;
    
    pair<bool, bool> propagation = cell.cover();
    bool propagates_horizontally = propagation.first;
    bool propagates_vertically = propagation.second;

    int score = 0;

    if(propagates_horizontally) {
        if(propagate(position, Horizontal)) score++;
    }

    if(propagates_vertically) {
        if(propagate(position, Vertical)) score++;
    }

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

int Board::getHeight() const {
    return height;
}

int Board::getWidth() const {
    return width;
}
