#include <iostream> // TODO remove (as of now only used for debug purposes)
#include "board.h"

using namespace std;

Board::Board(): Board(15, 15) {}

Board::Board(int width, int height): 
  width(width), 
  height(height), 
  grid(height, vector<char>(width, EMPTY_CELL)) 
{}

bool Board::addWord(Position pos, Orientation orientation, std::string word) {
    // TODO check if everything is valid
    for(int i = 0; i < word.size(); i++) {
        grid[pos.getY()][pos.getX()] = word[i];
        pos.stepForward(orientation);
    }

    return true;
}

bool Board::setWidth(int width) {
    if(width > 0 && width <= 20) {
        this->width = width;
        for(auto &row: grid) {
            row.resize(width, EMPTY_CELL);
        }
        return true;
    } else {
        return false;
    }
}

bool Board::setHeight(int height) {
    if(height > 0 && height <= 20) {
        this->height = height;
        grid.resize(height, vector<char>(width, EMPTY_CELL));
        return true;
    } else {
        return false;
    }
}

void Board::printGrid(ostream &out) const {
    out << "  ";
    for(int i = 0; i < width; i++) {
        out << (char)('a' + i) << ' ';
    }
    out << endl;
    for(int j = 0; j < height; j++) {
        out << (char)('A' + j) << ' ';
        for(int i = 0; i < width; i++) {
            char cell = grid[j][i];

            if(cell == EMPTY_CELL) out << "  ";
            else out << grid[j][i] << ' ';
        }
        out << endl;
    }
}
