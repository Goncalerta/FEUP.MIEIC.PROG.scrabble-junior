#pragma once
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

public:
    Command(char cx, char cy, char corientation, std::string cword);
    bool isValid();

    Position getPos();
    Orientation getOrientation();
    std::string getWord();
};

#endif