#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <string>
#include "orientation.h"
#include "position.h"
#include "cursor.h"
#include "command.h"

class Word {
    static const char* DICTIONARY;

    Position start;
    Orientation orientation;
    std::string word;

    public:
    Word(Position start, Orientation orientation, std::string word);
    Word(Command cmd);

    void printToStream(std::ostream &out) const;
    std::string getWord() const;
    Cursor getCursorAtStart() const;
    int size() const; // unused right now
    bool inDict() const;
};

#endif