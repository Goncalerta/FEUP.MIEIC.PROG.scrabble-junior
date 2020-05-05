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
        int player_number = game.getCurrentPlayerNumber();

        // TODO ENFORCE TWO MOVES WHEN POSSIBLE
        if(game.canCurrentPlayerMove()) {
            vector<Position> edge_case_legal_positions;
            if(game.mustPlayTwiceEdgeCase(edge_case_legal_positions)) {
                displayer.draw(edge_case_legal_positions);
                cout << "Input a valid position in the board to play: ";
                has_input = getline(cin, p_input).good();
                displayer.clearErrors();

                if(p_input.size() > 2) {
                    displayer.pushError("Too many characters in input.");
                    continue;
                }

                if(p_input.size() < 2) {
                    displayer.pushError("Too few characters in input.");
                    continue;
                }

                if(!Position::isValid(p_input[1], p_input[0])) {
                    displayer.pushError("Couldn't parse input as a position.");
                    continue;
                }

                Position pos(p_input[1], p_input[0]);
                game.move(pos, displayer, edge_case_legal_positions); // TODO remove bool?
                if(game.getMovesLeftThisTurn() == 0) {
                    game.nextTurn();
                }
            } else {
                displayer.draw();
                cout << "Input a valid position in the board to play: ";
                has_input = getline(cin, p_input).good();
                displayer.clearErrors();

                if(p_input.size() > 2) {
                    displayer.pushError("Too many characters in input.");
                    continue;
                }

                if(p_input.size() < 2) {
                    displayer.pushError("Too few characters in input.");
                    continue;
                }

                if(!Position::isValid(p_input[1], p_input[0])) {
                    displayer.pushError("Couldn't parse input as a position.");
                    continue;
                }

                Position pos(p_input[1], p_input[0]);
                game.move(pos, displayer); // TODO remove bool?
                if(game.getMovesLeftThisTurn() == 0) {
                    game.nextTurn();
                }
            }
        } else if(game.getMovesThisTurn() >= 1) {
            stringstream error;
            error << "Player " << player_number << " couldn't make any more moves this turn.";
            displayer.pushError(error.str().c_str());

            game.nextTurn();
        } else if(game.getPool().size() >= 2) {
            stringstream error;
            error << "Player " << player_number << " couldn't make any move." << endl
                << "Must exchange two letters with the Pool this turn.";
            displayer.pushError(error.str().c_str());

            displayer.draw();
            cout << "Input two letters to exchange: ";
            has_input = getline(cin, p_input).good();
            displayer.clearErrors();

            char letter1, letter2;
            stringstream input_stream(p_input);
            input_stream >> letter1 >> letter2;

            if(input_stream.fail() || input_stream.str().empty()) {
                // TODO maybe something more efficient that .str() ?
                displayer.pushError("Invalid input.");
                continue;
            }

            if(game.exchange(letter1, letter2, displayer, rng)) {
                game.nextTurn();
            }
        } else if(game.getPool().size() == 1) {
            stringstream error;
            error << "Player " << player_number << " couldn't make any move." << endl
                << "Must exchange a letter with the remaining one in Pool this turn.";
            displayer.pushError(error.str().c_str());

            displayer.draw();
            cout << "Input a letter to exchange: ";
            has_input = getline(cin, p_input).good();
            displayer.clearErrors();

            char letter;
            stringstream input_stream(p_input);
            input_stream >> letter;

            if(input_stream.fail() || input_stream.str().empty()) {
                // TOD maybe something more efficient that .str() ?
                displayer.pushError("Invalid input.");
                continue;
            }

            if(game.exchange(letter, displayer, rng)) {
                game.nextTurn();
            }
        } else {
            stringstream error;
            error << "Player " << player_number << " couldn't make any move." << endl
                << "The pool is empty. Turn has been skipped.";
            displayer.pushError(error.str().c_str());

            game.nextTurn();
        }
    } while(has_input && !game.isOver());

    // TODO gameover screen
    cout << "GAME OVER." << endl << "WINNER: PLAYER " << game.getLeadingScorePlayerNumber() << endl;
    cout << "Press ENTER to exit . . .";
    cin.get();

    return 0;
}
