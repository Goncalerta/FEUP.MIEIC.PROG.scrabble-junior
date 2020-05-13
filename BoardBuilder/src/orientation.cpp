#include <cctype>
#include "orientation.h"

using namespace std;

Orientation parseOrientation(char c) {
    if(toupper(c) == 'H') return Horizontal;
    else return Vertical;
}

ostream& operator<<(ostream &out, Orientation orientation) {
    if(orientation == Horizontal) out << 'H';
    else out << 'V';
    return out;
}
