#include <fstream>
#include "word.h"
#include "board.h"

using namespace std;

const char* Word::DICTIONARY = "WORDS.txt";

Word::Word(Position start, Orientation orientation, string word): 
  start(start),
  orientation(orientation),
  word(word) {}

Word::Word(Command cmd): Word(cmd.getPos(), cmd.getOrientation(), cmd.getWord()) {}

void Word::printToStream(ostream &out) const {
    out << start.getYChar() << start.getXChar() << ' ';
    out << orientationToChar(orientation);
    out << ' ' << word << endl;
}

std::string Word::getWord() const {
    return word;
}

Cursor Word::getCursorAtStart() const {
    return Cursor(start, orientation);
}

int Word::size() const {
    return word.size();
}

bool Word::inDict() const {
    // ifstream dict(DICTIONARY, std::ifstream::ate);
    // while(dict)
    //     cout << dict;
    // TODO
    return true;
}
