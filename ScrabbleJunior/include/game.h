#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include "player.h"
#include "board.h"
#include "pool.h"
#include "position.h"

// TODO a state machine would probably be better than an enum
enum State {
    Unstarted,
    Ongoing,
    Finished
};

class Game {
    Board board;
    std::vector<Player> players;
    Pool pool;
    int turn;
    State state;

    public:
    Game(int num_players);
    bool loadBoardFile(std::istream &board_file);
    void startGame(std::default_random_engine &rng);

    const std::vector<Player>& getPlayers() const;
    const Board& getBoard() const;

    Player& getCurrentPlayer();
    bool move(Position position, std::ostream &out);
    void nextTurn();

    void printGame(std::ostream &out);
};

#endif