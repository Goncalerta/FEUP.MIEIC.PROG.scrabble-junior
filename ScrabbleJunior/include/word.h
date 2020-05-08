#ifndef WORD_H
#define WORD_H

#include <string>
#include "position.h"
#include "orientation.h"

class Word {
    const Position start;
    const Orientation orientation;
    const std::string word;

    public:
    typedef std::string::const_iterator const_iterator;
    Word(Position start, Orientation orientation, std::string &word);

    Position getStart() const;
    Orientation getOrientation() const;

    const_iterator begin() const;
    const_iterator end() const;
};

#endif