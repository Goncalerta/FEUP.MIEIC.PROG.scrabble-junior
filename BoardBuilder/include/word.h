<<<<<<< HEAD
#pragma once
=======
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
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

<<<<<<< HEAD
public:
    Word(Position start, Orientation orientation, std::string word);
    Word(Command cmd);

    void printToStream(std::ostream& out) const;
=======
    public:
    Word(Position start, Orientation orientation, std::string word);
    Word(Command cmd);

    void printToStream(std::ostream &out) const;
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
    std::string getWord() const;
    Cursor getCursorAtStart() const;
    int size() const; // unused right now
    bool inDict() const;
};

#endif