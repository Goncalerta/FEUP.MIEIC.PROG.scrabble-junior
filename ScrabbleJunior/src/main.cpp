#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include "board.h"
#include "pool.h"
#include "orientation.h"
#include "position.h"
#include "player.h"
#include "game.h"
#include "displayer.h"

using namespace std;

int main() {
    srand(time(0));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    // TODO ask number of players
    // TODO ask name of the board file 
    string board_file_name = "TEST.txt";
    ifstream board_file(board_file_name);

    Game game(2);
    GameDisplayer displayer(game);

    if(!game.loadBoardFile(board_file)) {
        cout << "Invalid board file";
        return 1; // TODO 
    }

    game.startGame(rng);

    string p_input;
    int plays = 2;

    bool has_input;

    do {
        displayer.draw();
        has_input = getline(cin, p_input).good();

        displayer.clearErrors();

        if(p_input.size() != 2) {
            displayer.pushError("Too many characters in input.");
            continue;
        }

        if(!Position::isValid(p_input[1], p_input[0])) {
            displayer.pushError("Couldn't parse input as a position.");
            continue;
        }

        Position pos(p_input[1], p_input[0]);
        if(game.move(pos, displayer)) plays -= 1;
        if(plays == 0) game.nextTurn();
    } while(has_input);

    return 0;
}
