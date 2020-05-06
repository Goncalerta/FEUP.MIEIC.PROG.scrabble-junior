#include <algorithm>
#include "player.h"

using namespace std;

Player::Player(int id): 
    id(id), 
    score(0), 
    hand{EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND, EMPTY_HAND} 
{}

void Player::refillHand(Pool &pool) {
    for(auto &letter: hand) {
        if(pool.isEmpty()) break;
        if(letter == EMPTY_HAND)  {
            letter = pool.popLetter();
        }
    }
}

void Player::exchange(Pool &pool, char letter) {
    char *hand_index = find(begin(hand), end(hand), letter);
    pool.exchange(hand_index);
}

void Player::exchange(Pool &pool, char letter1, char letter2) {
    char *hand_index1, *hand_index2;
    hand_index1 = find(begin(hand), end(hand), letter1);

    if(letter1 == letter2) {
        // Avoid choosing the same letter twice.
        hand_index2 = find(hand_index1++, end(hand), letter2);
    } else {
        hand_index2 = find(begin(hand), end(hand), letter2);
    }

    pool.exchange(hand_index1, hand_index2);
}

bool Player::isValidLetter(char letter) {
    return letter >= 'A' && letter <= 'Z';
}

bool Player::hasLetter(char letter) {
    for(auto l = handBegin(); l <= handEnd(); l++) {
        if(*l == letter) return true;
    }

    return false;
}

bool Player::hasPair(char letter1, char letter2) {
    bool has_letter1 = false;
    bool has_letter2 = false;

    for(auto l = handBegin(); l <= handEnd(); l++) {
        if(*l == letter1 && !has_letter1) has_letter1 = true;
        else if(*l == letter2) has_letter2 = true;
    }

    return has_letter1 && has_letter2;
}

void Player::useLetter(char letter) {
    for(auto l = begin(hand); l <= end(hand); l++) {
        if(*l == letter) {
            *l = EMPTY_HAND;
            return;
        }
    }
}

int Player::getId() const {
    return id;
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int score) {
    this->score += score;
}

const char* Player::handBegin() const {
    return cbegin(hand);
}

const char* Player::handEnd() const {
    return cend(hand);
}
