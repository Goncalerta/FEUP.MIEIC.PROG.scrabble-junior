#ifndef HAND_H
#define HAND_H

#include <functional>
#include <ostream>
#include "pool.h"

class Hand {
    friend std::ostream& operator<<(std::ostream& out, const Hand& hand); 
    
    static const int HAND_SIZE = 7;
    static const char EMPTY = '_';

    char hand[HAND_SIZE];

    int indexOf(char* element) const;

    public:
    typedef std::function<void (int, char)> SwapLetterAnimator;

    Hand();

    bool isFull() const;
    void refill(Pool &pool, SwapLetterAnimator swap_hand = nullptr);
    void exchange(Pool &pool, char letter, SwapLetterAnimator swap_hand = nullptr);
    void exchange(Pool &pool, char letter1, char letter2, SwapLetterAnimator swap_hand = nullptr);

    bool hasLetter(char letter) const;
    int countLetter(char letter) const;
    void useLetter(char letter);

    const char* begin() const;
    const char* end() const;
};

std::ostream& operator<<(std::ostream& out, const Hand& hand); 

#endif