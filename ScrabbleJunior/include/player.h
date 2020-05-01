#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iterator>
#include "pool.h"

class Player {
    static const int TILES_PER_PLAYER = 7;
    
    int score;
    char hand[TILES_PER_PLAYER];

    public:
    static const char EMPTY_HAND = '\0';
    
    Player();
    void refillHand(Pool &pool);
    bool hasLetter(char letter);
    void useLetter(char letter);
    void addScore(int score);

    const char* handBegin() const;
    const char* handEnd() const;
};

#endif