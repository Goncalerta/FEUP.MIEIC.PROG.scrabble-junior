#include "board.h"

using namespace std;

Board::Board(): Board(15, 15) {}

Board::Board(int width, int height): 
  width(width), 
  height(height), 
  words({}), 
  grid(height, vector<char>(width, '\0'))
{}

bool Board::addWord(Word word) {
    // TODO check if word already exists (dont duplicate it)
    string w = word.getWord();
    Cursor cursor = word.getCursorAtStart();
    cursor.stepBackwards();
    if(cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
        return false;
    }

    for(int i = 0; i < w.size(); i++) {
        cursor.stepForward();
        if(!cursor.inRect(Position(0, 0), width, height)) {
            return false;
        }
        char current_char = grid[cursor.getY()][cursor.getX()];

        if(current_char == '\0') {
            cursor.stepLateral(1);
            if(cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
                return false;
            }
            cursor.stepLateral(-2);
            if(cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
                return false;
            }
            cursor.stepLateral(1);
        } else if(current_char != w[i]) {
            return false;
        }
    }
    cursor.stepForward();
    if(cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
        return false;
    }

    cursor = word.getCursorAtStart();
    for(int i = 0; i < w.size(); i++) {
        grid[cursor.getY()][cursor.getX()] = w[i];
        cursor.stepForward();
    }

    words.push_back(word);
    return true;
}

bool Board::setWidth(int width) {
    if(width > 0 && width <= 20) {
        this->width = width;
        return true;
    } else {
        return false;
    }
}

bool Board::setHeight(int height) {
    if(height > 0 && height <= 20) {
        this->height = height;
        return true;
    } else {
        return false;
    }
}

void Board::printData(ostream &out) const {
    out << height << " x " << width << endl;
    for(int i = 0; i < words.size(); i++) {
        words[i].printToStream(out);
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

            if(cell == '\0') out << "  ";
            else out << grid[j][i] << ' ';
        }
        out << endl;
    }
}
