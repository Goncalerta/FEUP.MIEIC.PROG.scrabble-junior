#include <algorithm>
#include "command.h"
using namespace std;

Command::Command(char cx, char cy, char corientation, string cword) {
    valid = false;
    if(cx < 'a' || cx > 'z') return;
    if(cy < 'A' || cy > 'Z') return;

    corientation = toupper(corientation);
    if(corientation != 'H' && corientation != 'V') return;

    valid = true;

    pos = Position(cx, cy);

    if(corientation == 'H') {
        orientation = Horizontal;
    } else if(corientation == 'V') {
        orientation = Vertical;
    }

    word = cword;
    transform(word.begin(), word.end(), word.begin(), toupper);
}

bool Command::isValid() {
    return valid;
}

Position Command::getPos() {
    return pos;
}

Orientation Command::getOrientation() {
    return orientation;
}

string Command::getWord() {
    return word;
}
