<<<<<<< HEAD
#pragma once
=======
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include "orientation.h"
#include "position.h"

class Command {
    Position pos;
    Orientation orientation;
    std::string word;

    bool valid;

<<<<<<< HEAD
public:
    Command(char cx, char cy, char corientation, std::string cword);
    bool isValid();

=======
    public:
    Command(char cx, char cy, char corientation, std::string cword);
    bool isValid();
    
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
    Position getPos();
    Orientation getOrientation();
    std::string getWord();
};

#endif