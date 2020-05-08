#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"

class Player {
    unsigned int score;
    unsigned int id;
    Hand hand;

    public:    
    Player(unsigned int id);

    const Hand& getHand() const;
    Hand& getHand();

    unsigned int getId() const;

    unsigned int getScore() const;
    void addScore(unsigned int score);
};

#endif