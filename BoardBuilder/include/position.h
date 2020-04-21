#ifndef POSITION_H
#define POSITION_H

class Position {
    protected:
    int x;
    int y;

    public:
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