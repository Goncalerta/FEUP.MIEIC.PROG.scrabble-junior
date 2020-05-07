#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iterator>
#include "pool.h"

#include <functional>
typedef std::function<void (int, char)> SwapHandAnimator;

class Player {
    static const int TILES_PER_PLAYER = 7;
    
    int score;
    int id;
    // TODO Hand class
    char hand[TILES_PER_PLAYER];

    public:
    static const char EMPTY_HAND = '_';
    static bool isValidLetter(char letter);
    
    Player(int id);
    bool needsRefill();
    void refillHand(Pool &pool, SwapHandAnimator swap_hand = nullptr);
    void exchange(Pool &pool, char letter, SwapHandAnimator swap_hand = nullptr);
    void exchange(Pool &pool, char letter1, char letter2, SwapHandAnimator swap_hand = nullptr);

    bool hasLetter(char letter);
    bool hasPair(char letter1, char letter2);
    void useLetter(char letter);

    int getId() const;

    int getScore() const;
    void addScore(int score);

    const char* handBegin() const;
    const char* handEnd() const;
};

#endif