#include "pool.h"
#include <algorithm>

using namespace std;

Pool::Pool() {}

void Pool::pushLetter(char c) {
    letters.push_back(c);
}

void Pool::pushWord(string s) {
    for(auto c: s) {
        pushLetter(c);
    }
}

char Pool::popLetter() {
    char letter = letters.back();
    letters.pop_back();
    return letter;
}

void Pool::shuffle(default_random_engine &rng) {
    std::shuffle(letters.begin(), letters.end(), rng);
}

bool Pool::isEmpty() const {
    return letters.empty();
}
