#include "player.h"

using namespace std;

Player::Player(): score(0), hand{EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND} {}

void Player::refillHand(Pool &pool) {
    for(auto &letter: hand) {
        if(pool.isEmpty()) break;
        if(letter == EMPTY_HAND)  {
            letter = pool.popLetter();
        }
    }
}

char Player::takeHand(int n) {
    // if(n >= 7 || n < 0) TODO ERROR
    char letter = hand[n];
    hand[n] = EMPTY_HAND;
    return letter;
} 

const char* Player::hand_begin() const {
    return cbegin(hand);
}

const char* Player::hand_end() const {
    return cend(hand);
}
