<<<<<<< HEAD
#pragma once
=======
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
#ifndef POSITION_H
#define POSITION_H

class Position {
<<<<<<< HEAD
protected:
    int x;
    int y;

public:
=======
    protected:
    int x;
    int y;

    public:
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
    Position();
    Position(int x, int y);
    Position(char x, char y);

    int getX() const;
    int getY() const;
    char getXChar() const;
    char getYChar() const;

    bool inRect(Position start, int width, int height) const;
};

#endif