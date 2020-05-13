#include <fstream>
#include <algorithm>
#include "word.h"
#include "board.h"

using namespace std;

Word::Word(Position start, Orientation orientation, string &word): 
  start(start),
  orientation(orientation),
  word(move(word)) 
{}

Position Word::getStart() const {
    return start;
}

Orientation Word::getOrientation() const {
    return orientation;
}

Word::const_iterator Word::begin() const {
    return word.begin();
}

Word::const_iterator Word::end() const {
    return word.end();
}

bool Word::inDict(istream &dict) const {
    string dict_word;

    while(dict >> dict_word) {
        transform(dict_word.begin(), dict_word.end(), dict_word.begin(), toupper);
        if(word == dict_word) return true;
    } 
    
    return false;
}

ostream& operator<<(ostream &out, const Word &word) {
    out << word.start << ' ' << word.orientation << ' ' << word.word;
    return out;
}
