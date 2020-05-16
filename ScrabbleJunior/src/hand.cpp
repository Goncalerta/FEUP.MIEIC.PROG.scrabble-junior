#include <iterator>
#include <algorithm>
#include "hand.h"

Hand::Hand() {
    std::fill(std::begin(hand), std::end(hand), EMPTY);
}

int Hand::indexOf(char *element) const {
    return element - std::begin(hand);
}

bool Hand::isFull() const {
    return std::find(std::begin(hand), std::end(hand), EMPTY) == std::end(hand);
}

void Hand::refill(Pool &pool, SwapLetterAnimator swap_hand) {
    for(auto &letter: hand) {
        if(pool.isEmpty()) break;
        
        if(letter == EMPTY) {
            letter = pool.popLetter();
            if(swap_hand) swap_hand(indexOf(&letter), letter);
        }
    }
}

void Hand::exchange(Pool &pool, char letter, SwapLetterAnimator swap_hand) {
    char *element = std::find(std::begin(hand), std::end(hand), letter);
    pool.exchange(element);
    if(swap_hand) swap_hand(indexOf(element), *element);
}

void Hand::exchange(Pool &pool, char letter1, char letter2, SwapLetterAnimator swap_hand) {
    char *element1, *element2;
    element1 = std::find(std::begin(hand), std::end(hand), letter1);

    if(letter1 == letter2) {
        // Avoids choosing the same letter twice.
        element2 = std::find(element1+1, std::end(hand), letter2);
    } else {
        element2 = std::find(std::begin(hand), std::end(hand), letter2);
    }

    pool.exchange(element1, element2);
    if(swap_hand) {
        swap_hand(element1 - std::begin(hand), *element1);
        swap_hand(element2 - std::begin(hand), *element2);
    }
}

bool Hand::hasLetter(char letter) const {
    return std::find(std::begin(hand), std::end(hand), letter) != std::end(hand);
}

int Hand::countLetter(char letter) const {
    return std::count(std::begin(hand), std::end(hand), letter);
}

void Hand::useLetter(char letter) {
    char *l = std::find(std::begin(hand), std::end(hand), letter);
    if(l != std::end(hand)) *l = EMPTY;
}

const char* Hand::begin() const {
    return std::cbegin(hand);
}

const char* Hand::end() const {
    return std::cend(hand);
}

std::ostream& operator<<(std::ostream& out, const Hand& hand) {
    for(const char &letter: hand) {
        out << letter << " ";
    }
    
    return out;
}
