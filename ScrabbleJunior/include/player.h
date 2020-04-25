#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iterator>
// #include <random>
#include "pool.h"

class Player {
    static const int TILES_PER_PLAYER = 7;
    static const int EMPTY_HAND = '\0';
    
    int score;
    char hand[TILES_PER_PLAYER];

    public:
    Player();
    void refillHand(Pool &pool);
    char takeHand(int n);
    const char* hand_begin() const;
    const char* hand_end() const;
};

#endif