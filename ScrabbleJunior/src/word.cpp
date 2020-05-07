#include "word.h"

using namespace std;

Word::Word(Position start, Orientation orientation, std::string word):
    start(start),
    orientation(orientation),
    word(word)
{}

Position Word::getStart() const {
    return start;
}

const string& Word::getWord() const {
    return word;
}

Orientation Word::getOrientation() const {
    return orientation;
}
