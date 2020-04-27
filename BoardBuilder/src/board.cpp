#include "board.h"

using namespace std;

<<<<<<< HEAD
Board::Board() : Board(15, 15) {}

Board::Board(int width, int height) :
    width(width),
    height(height),
    words({}),
    grid(height, vector<char>(width, '\0'))
=======
Board::Board(): Board(15, 15) {}

Board::Board(int width, int height): 
  width(width), 
  height(height), 
  words({}), 
  grid(height, vector<char>(width, '\0'))
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
{}

bool Board::addWord(Word word) {
    // TODO check if word already exists (dont duplicate it)
    string w = word.getWord();
    Cursor cursor = word.getCursorAtStart();
    cursor.stepBackwards();
<<<<<<< HEAD
    if (cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
        return false;
    }

    for (int i = 0; i < w.size(); i++) {
        cursor.stepForward();
        if (!cursor.inRect(Position(0, 0), width, height)) {
=======
    if(cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
        return false;
    }

    for(int i = 0; i < w.size(); i++) {
        cursor.stepForward();
        if(!cursor.inRect(Position(0, 0), width, height)) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            return false;
        }
        char current_char = grid[cursor.getY()][cursor.getX()];

<<<<<<< HEAD
        if (current_char == '\0') {
            cursor.stepLateral(1);
            if (cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
                return false;
            }
            cursor.stepLateral(-2);
            if (cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
                return false;
            }
            cursor.stepLateral(1);
        }
        else if (current_char != w[i]) {
=======
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
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            return false;
        }
    }
    cursor.stepForward();
<<<<<<< HEAD
    if (cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
=======
    if(cursor.inRect(Position(0, 0), width, height) && grid[cursor.getY()][cursor.getX()] != '\0') {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        return false;
    }

    cursor = word.getCursorAtStart();
<<<<<<< HEAD
    for (int i = 0; i < w.size(); i++) {
=======
    for(int i = 0; i < w.size(); i++) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        grid[cursor.getY()][cursor.getX()] = w[i];
        cursor.stepForward();
    }

    words.push_back(word);
    return true;
}

bool Board::setWidth(int width) {
<<<<<<< HEAD
    if (width > 0 && width <= 20) {
        this->width = width;
        return true;
    }
    else {
=======
    if(width > 0 && width <= 20) {
        this->width = width;
        return true;
    } else {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        return false;
    }
}

bool Board::setHeight(int height) {
<<<<<<< HEAD
    if (height > 0 && height <= 20) {
        this->height = height;
        return true;
    }
    else {
=======
    if(height > 0 && height <= 20) {
        this->height = height;
        return true;
    } else {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        return false;
    }
}

<<<<<<< HEAD
void Board::printData(ostream& out) const {
    out << height << " x " << width << endl;
    for (int i = 0; i < words.size(); i++) {
=======
void Board::printData(ostream &out) const {
    out << height << " x " << width << endl;
    for(int i = 0; i < words.size(); i++) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        words[i].printToStream(out);
    }
}

<<<<<<< HEAD
void Board::printGrid(ostream& out) const {
    out << "  ";
    for (int i = 0; i < width; i++) {
        out << (char)('a' + i) << ' ';
    }
    out << endl;
    for (int j = 0; j < height; j++) {
        out << (char)('A' + j) << ' ';
        for (int i = 0; i < width; i++) {
            char cell = grid[j][i];

            if (cell == '\0') out << "  ";
=======
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
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            else out << grid[j][i] << ' ';
        }
        out << endl;
    }
<<<<<<< HEAD
}
=======
}
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
