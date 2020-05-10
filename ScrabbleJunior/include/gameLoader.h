#ifndef GAME_LOADER_H
#define GAME_LOADER_H

#include <fstream>
#include <random>
#include "cmd.h"
#include "board.h"
#include "pool.h"

enum LoadState {
    FailedToLoad,
    Unloaded,
    BoardLoaded,    
    ReadyToPlay
};

class GameLoader {
    static const Color TEXT_COLOR;
    static const Color ERROR_COLOR;

    int max_players;
    LoadState state;

    bool openBoardFile(std::ifstream &board_file);
    bool loadBoardFile(Board &board, std::ifstream &board_file);

    public:
    GameLoader();

    LoadState getState() const;

    void loadBoard(Board &board);
    void promptNumberPlayers(int &num_players);
};

#endif