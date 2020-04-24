#pragma once
#ifndef CURSOR_H
#define CURSOR_H

#include <iostream>
#include <string>
#include "orientation.h"
#include "position.h"

class Cursor : public Position {
    Orientation orientation;

public:
    Cursor(int x, int y, Orientation orientation);
    Cursor(Position pos, Orientation orientation);
    void stepForward();
    void stepBackwards();
    void stepLateral(int n);
};

#endif