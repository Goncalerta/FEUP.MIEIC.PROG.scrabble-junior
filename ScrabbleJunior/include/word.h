#ifndef WORD_H
#define WORD_H

#include <string>
#include "position.h"
#include "orientation.h"

class Word {
    Position start;
    Orientation orientation;
    std::string word;

    public:
    Word(Position start, Orientation orientation, std::string word);

    Position getStart() const;
    const std::string& getWord() const;
    Orientation getOrientation() const;
};

#endif