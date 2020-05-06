#ifndef GAME_H
#define GAME_H

class Game;

#include <string>
#include <vector>
#include <iostream>
#include <random>
#include "player.h"
#include "board.h"
#include "pool.h"
#include "position.h"
#include "displayer.h"

class Game {
    Board &board;
    std::vector<Player> players;
    Pool pool;
    unsigned int turn;
    unsigned int moves_left;

    public:
    Game(Board &board, unsigned int num_players, std::default_random_engine &rng);

    const std::vector<Player>& getPlayers() const;
    const Board& getBoard() const;
    const Pool& getPool() const;

    Player& getCurrentPlayer(); // TODO make const
    int getCurrentPlayerNumber() const;
    int getMovesLeftThisTurn() const;
    int getMovesThisTurn() const;
    bool canCurrentPlayerMove(); // TODO make const
    
    bool isOver() const;
    std::vector<const Player*> getLeaderboard() const;
    int getLeadingScorePlayerNumber() const;

    bool move(Position position, GameDisplayer &displayer);
    bool move(Position position, GameDisplayer &displayer, std::vector<Position> &legal_moves);
    bool exchange(char letter1, GameDisplayer &displayer, std::default_random_engine &rng);
    bool exchange(char letter1, char letter2, GameDisplayer &displayer, std::default_random_engine &rng);
    void nextTurn();

    bool mustPlayTwiceEdgeCase(std::vector<Position> &positions);
};

#endif