#include <algorithm>
#include "command.h"
using namespace std;

Command::Command(char cx, char cy, char corientation, string cword) {
    valid = false;
<<<<<<< HEAD
    if (cx < 'a' || cx > 'z') return;
    if (cy < 'A' || cy > 'Z') return;

    corientation = toupper(corientation);
    if (corientation != 'H' && corientation != 'V') return;
=======
    if(cx < 'a' || cx > 'z') return;
    if(cy < 'A' || cy > 'Z') return;

    corientation = toupper(corientation);
    if(corientation != 'H' && corientation != 'V') return;
>>>>>>> af2634ed998871d93345b34770acad66809fd03f

    valid = true;

    pos = Position(cx, cy);

<<<<<<< HEAD
    if (corientation == 'H') {
        orientation = Horizontal;
    }
    else if (corientation == 'V') {
=======
    if(corientation == 'H') {
        orientation = Horizontal;
    } else if(corientation == 'V') {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
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
<<<<<<< HEAD
}
=======
}
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
