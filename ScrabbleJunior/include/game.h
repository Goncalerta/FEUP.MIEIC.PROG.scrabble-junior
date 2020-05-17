#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <ostream>
#include <random>
#include "player.h"
#include "board.h"
#include "pool.h"
#include "position.h"
#include "gameDisplayer.h"

enum TurnState {
    MUST_MOVE,
    MUST_EXCHANGE_TWO,
    MUST_EXCHANGE_ONE,
    MUST_END_TURN,
    MUST_SKIP_TURN,
};

class Game {
    Board &board;
    Pool pool;
    std::vector<Player> players;
    GameDisplayer displayer;
    unsigned int current_player_index;
    unsigned int moves_left;

    bool isOver() const;
    const Player& getCurrentPlayer() const; 
    Player& getCurrentPlayer();
    std::vector<int> getWinnersId() const;

    TurnState getTurnState() const;
    bool playLoop(std::default_random_engine &rng);

    void getCheckLegalMove(bool &must_play_twice, GameDisplayer::CheckLegalMove &is_legal) const;
    bool parsePosition(std::istream &input, Position &position);
    bool validateMove(Position position, bool must_play_twice, GameDisplayer::CheckLegalMove is_legal);
    void move(Position position);
    
    bool parseLetter(std::istream &input, char &letter);
    bool validateExchange(char letter);
    void exchange(char letter, std::default_random_engine &rng);
    
    bool parseLetters(std::istream &input, char &letter1, char &letter2);
    bool validateExchange(char letter1, char letter2);
    void exchange(char letter1, char letter2, std::default_random_engine &rng);

    void nextTurn();

    public:
    Game(Board &board, unsigned int num_players);

    bool play(std::default_random_engine &rng);
};

#endif