#ifndef DISPLAYER_H
#define DISPLAYER_H

class GameDisplayer;

#include <string>
#include <vector>
#include "game.h"

class GameDisplayer {
    std::vector<std::string> error_messages;
    Game &game;

    void drawBoard(const Board &board, const Player &current_player);
    void drawPlayers(const std::vector<Player> &players, int x_offset);
    void drawCurrentPlayer();
    void drawErrorMessages();

    public:
    GameDisplayer(Game &game);

    void pushError(const char *error);
    void clearErrors();

    void draw();
};

#endif