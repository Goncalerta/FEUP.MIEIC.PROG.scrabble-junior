#include <fstream>
#include <algorithm>
#include "word.h"
#include "board.h"

using namespace std;

const char* Word::DICTIONARY = "WORDS.txt";

<<<<<<< HEAD
Word::Word(Position start, Orientation orientation, string word) :
    start(start),
    orientation(orientation),
    word(word) {}

Word::Word(Command cmd) : Word(cmd.getPos(), cmd.getOrientation(), cmd.getWord()) {}

void Word::printToStream(ostream& out) const {
=======
Word::Word(Position start, Orientation orientation, string word): 
  start(start),
  orientation(orientation),
  word(word) {}

Word::Word(Command cmd): Word(cmd.getPos(), cmd.getOrientation(), cmd.getWord()) {}

void Word::printToStream(ostream &out) const {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
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
    ifstream dict(DICTIONARY);
    string dict_word;

<<<<<<< HEAD
    while (dict >> dict_word) {
        transform(dict_word.begin(), dict_word.end(), dict_word.begin(), toupper);
        if (word == dict_word) return true;
    }

    return false;
}
=======
    while(dict >> dict_word) {
        transform(dict_word.begin(), dict_word.end(), dict_word.begin(), toupper);
        if(word == dict_word) return true;
    } 
    
    return false;
}
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
