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

pair<char, char> Pool::exchange(std::pair<char, char> p) {
    char c1, c2;
    if(letters.empty()) return p;
    
    c1 = letters.back();
    letters.pop_back();

    if(letters.empty()) {
        letters.push_back(p.first);
        return pair<char, char>(c1, p.second);
    }

    c2 = letters.back();
    letters.pop_back();

    letters.push_back(p.first);
    letters.push_back(p.second);
    return pair<char, char>(c1, c2);
}
