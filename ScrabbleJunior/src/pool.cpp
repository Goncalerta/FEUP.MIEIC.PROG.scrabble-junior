#include "pool.h"
#include <algorithm>

using namespace std;

Pool::Pool() {}

void Pool::fill(std::vector<char> &letters) {
    this->letters.swap(letters);
}

void Pool::pushLetter(char c) {
    letters.push_back(c);
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

int Pool::size() const {
    return letters.size();
}

void Pool::exchange(char *letter) {
    swap(letters[0], *letter);
}

void Pool::exchange(char *letter1, char *letter2) {
    swap(letters[0], *letter1);
    swap(letters[1], *letter2);
}
