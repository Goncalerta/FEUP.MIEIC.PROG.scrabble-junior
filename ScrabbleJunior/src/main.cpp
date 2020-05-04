#include <iostream>
#include <fstream>
#include <sstream>
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

    bool has_input;

    do {
        if(!game.canCurrentPlayerMove()) {
            // TODO ENFORCE TWO MOVES WHEN POSSIBLE
            int player_number = game.getCurrentPlayerNumber();
            
            if(game.getMovesThisTurn() == 0) {
                stringstream error;
                error << "Player " << player_number << " couldn't make any move.";
                displayer.pushError(error.str().c_str());

                if(game.getPool().isEmpty()) {
                    stringstream error;
                    error << "Pool is empty. Turn has been skipped.";
                    displayer.pushError(error.str().c_str());

                    game.nextTurn();
                    continue;
                } else {
                    stringstream error;
                    error << "Player exchanged two cells with the Pool and skipped their turn.";
                    displayer.pushError(error.str().c_str());

                    
                }
                // game.getCurrentPlayer().
            } else {
                stringstream error;
                error << "Player " << player_number << " couldn't make any more moves.";
                displayer.pushError(error.str().c_str());

                game.nextTurn();
                continue;
            }
        }
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
        if(game.move(pos, displayer)); // TODO remove?
        if(game.getMovesLeftThisTurn() == 0) {
            game.nextTurn();
        }
    } while(has_input);

    return 0;
}
