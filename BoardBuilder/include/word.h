#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <string>
#include "orientation.h"
#include "position.h"

class Word {
    friend std::ostream& operator<<(std::ostream &out, const Word &word);

    Position start;
    Orientation orientation;
    std::string word;

    public:
    typedef std::string::const_iterator const_iterator;
    Word(Position start, Orientation orientation, std::string &word);

    void printToStream(std::ostream &out) const;
    Position getStart() const;
    Orientation getOrientation() const;

    const_iterator begin() const;
    const_iterator end() const;
    bool inDict(std::istream &dict) const;
};

std::ostream& operator<<(std::ostream &out, const Word &word);

#endif