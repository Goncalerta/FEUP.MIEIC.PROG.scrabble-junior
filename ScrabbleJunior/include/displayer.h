#ifndef DISPLAYER_H
#define DISPLAYER_H

class GameDisplayer;

#include <string>
#include <vector>
#include "game.h"
#include "position.h"

class GameDisplayer {
    std::vector<std::string> error_messages;
    Game &game;

    void drawBoard(const Board &board);
    void drawBoard(const Board &board, const char *hand_begin, const char *hand_end);
    void drawBoard(const Board &board, const char *hand_begin, const char *hand_end, std::vector<Position> &legal_moves);
    void drawPlayers(const std::vector<Player> &players, int x_offset);
    void drawLeaderboard(std::vector<const Player*> players, int x_offset);
    void declareWinners(std::vector<const Player*> leaderboard);
    void drawCurrentPlayer();
    void drawErrorMessages();

    public:
    GameDisplayer(Game &game);

    void pushError(const char *error);
    void clearErrors();

    void draw();
    void draw(std::vector<Position> &legal_moves);
    void drawGameOver();
};

#endif