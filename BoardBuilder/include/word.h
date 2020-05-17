#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <string>
#include "orientation.h"
#include "position.h"

class Word {
    friend std::ostream& operator<<(std::ostream &out, const Word &word);

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

std::ostream& operator<<(std::ostream &out, const Word &word);

#endif