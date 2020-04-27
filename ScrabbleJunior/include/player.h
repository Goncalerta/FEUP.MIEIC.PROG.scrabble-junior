<<<<<<< HEAD
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

class Player {

}
=======
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iterator>
// #include <random>
#include "pool.h"

class Player {
    static const int TILES_PER_PLAYER = 7;
    static const char EMPTY_HAND = '\0';
    
    int score;
    char hand[TILES_PER_PLAYER];

    public:
    Player();
    void refillHand(Pool &pool);
    bool hasLetter(char letter);
    void useLetter(char letter);
    void addScore(int score);

    const char* handBegin() const;
    const char* handEnd() const;
};
>>>>>>> af2634ed998871d93345b34770acad66809fd03f

#endif