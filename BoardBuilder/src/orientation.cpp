#include <cctype>
#include "orientation.h"

using namespace std;

Orientation charToOrientation(char c) {
    if(c == 'H') return Horizontal;
    else return Vertical;
}

ostream& operator<<(ostream &out, Orientation orientation) {
    if(orientation == Horizontal) out << 'H';
    else out << 'V';
    return out;
}
