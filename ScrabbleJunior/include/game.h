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
#include "displayer.h"

class Game {
    Board &board;
    std::vector<Player> players;
    Pool pool;
    unsigned int current_player_index;
    unsigned int moves_left;

    bool validateMove(Position position, std::ostream &error_stream) const;
    void _move(Position position, GameDisplayer &displayer);

    public:
    Game(Board &board, unsigned int num_players, std::default_random_engine &rng);

    const std::vector<Player>& getPlayers() const;
    const Board& getBoard() const;
    const Pool& getPool() const; // TODO Do I need this?

    const Player& getCurrentPlayer() const;
    int getMovesLeftThisTurn() const; 
    
    bool isOver() const;
    std::vector<const Player*> getLeaderboard() const;

    bool move(Position position, GameDisplayer &displayer);
    bool move(Position position, GameDisplayer &displayer, std::vector<Position> &legal_moves); // TODO a better name for legal_moves
    bool exchange(char letter1, GameDisplayer &displayer, std::default_random_engine &rng);
    bool exchange(char letter1, char letter2, GameDisplayer &displayer, std::default_random_engine &rng);
    void nextTurn(GameDisplayer &displayer);

    bool mustPlayTwiceEdgeCase(std::vector<Position> &positions); // TODO better name/organization than edge case
};

#endif