<<<<<<< HEAD
#pragma once
=======
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
#ifndef CURSOR_H
#define CURSOR_H

#include <iostream>
#include <string>
#include "orientation.h"
#include "position.h"

<<<<<<< HEAD
class Cursor : public Position {
    Orientation orientation;

public:
=======
class Cursor: public Position {
    Orientation orientation;

    public:
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
    Cursor(int x, int y, Orientation orientation);
    Cursor(Position pos, Orientation orientation);
    void stepForward();
    void stepBackwards();
    void stepLateral(int n);
};

#endif