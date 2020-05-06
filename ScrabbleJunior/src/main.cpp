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
#include "cmd.h"
#include <algorithm>

using namespace std;

bool openBoardFile(ifstream &board_file) {
    string file_name;

    cout << "Input the name of your board: ";
    getline(cin, file_name);
    if(cin.fail()) return false;

    if(file_name.size() == 0) {
        setcolor(RED);
        cout << "Must input a file name." << endl;
        return false;
    }

    board_file.open(file_name, ios_base::in);
    if(!board_file.is_open()) {
        file_name += ".txt";
        board_file.open(file_name, ios_base::in);
        
        if(!board_file.is_open()) {
            setcolor(RED);
            cout << "File does not exist or is unavailable." << endl;
            return false;
        }
    }
    
    return true;
}

bool loadBoardFile(Board &board, std::istream &board_file) {
    board = Board();
    unsigned int width, height;

    board_file >> height;
    if(board_file.fail()) {
        setcolor(RED);
        cout << "Failed to parse height in given file." << endl;
        return false;
    }
    if(height == 0) {
        setcolor(RED);
        cout << "Height must not be zero." << endl;
        return false;
    }
    if(height > 20) {
        setcolor(RED);
        cout << "Max height is 20." << endl;
        return false;
    }

    char _x; // Ignored
    board_file >> _x;

    board_file >> width;
    if(board_file.fail()) {
        setcolor(RED);
        cout << "Failed to parse width in given file." << endl;
        return false;
    }
    if(width == 0) {
        setcolor(RED);
        cout << "Width must not be zero." << endl;
        return false;
    }
    if(width > 20) {
        setcolor(RED);
        cout << "Max width is 20." << endl;
        return false;
    }

    board.setSize(width, height);

    char c_position[2];
    char c_orientation;
    string word;

    while(board_file >> c_position >> c_orientation >> word) {
        if(!Position::isValid(c_position[1], c_position[0])) break;
        Position position(c_position[1], c_position[0]);
        
        Orientation orientation;
        if(c_orientation == 'H') orientation = Horizontal;
        else if(c_orientation == 'V') orientation = Vertical;
        else break;

        if(!board.addWord(position, orientation, word)) return false; // TODO ERROR MESSAGE
    }

    return true;
}

void playGame(Game &game, GameDisplayer displayer, default_random_engine rng) {
    string p_input;
    bool has_input;

    do {
        int player_number = game.getCurrentPlayerNumber();

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
}

bool promptPlayAgain() {
    string input;
    while(true) {
        cout << "Play again? (Y/N): ";
        getline(cin, input);
        if(cin.fail()) return false;

        char answer;
        stringstream input_stream(input);
        input_stream >> answer;

        if(input_stream.fail()) std::cout << "Invalid input.";
        else if(answer == 'Y' || answer == 'y') return true;
        else if(answer == 'N' || answer == 'n') return false;
        else std::cout << "Invalid input.";
    }
}

int playOnce(default_random_engine rng) {
    ifstream board_file;
    Board board;
    bool load_successful = false;

    while(!load_successful) {
        setcolor(LIGHTGRAY);
        if(!openBoardFile(board_file)) {
            if(cin.fail()) {
                setcolor(RED);
                cout << "stdin failed while trying to read file. Exiting . . ." << endl;
                setcolor(LIGHTGRAY);
                return 1;
            } else {
                continue;
            }
        }
        
        if(loadBoardFile(board, board_file)) {
            if(board.countLetters() >= 14) {
                load_successful = true;
            } else {
                setcolor(RED);
                cout << "Board must have at least 14 letters in order to be playable." << endl;    
            }
        } else {
            setcolor(RED);
            cout << "Unable to load board." << endl;
        }

        board_file.close();
    }

    string input_line;
    int num_players;
    bool valid_num_players = false;
    unsigned int max_players = min(4u, board.countLetters()/7);

    while(!valid_num_players) {
        setcolor(LIGHTGRAY);
        // TODO print board
        cout << "This board allows you to play a game with up to " << max_players << " players." << endl;
        cout << "Input the number of players (2-" << max_players << "): ";
        getline(cin, input_line);
        if(cin.fail()) {
            setcolor(RED);
            cout << "stdin failed while trying to read number of players. Exiting . . ." << endl;
            setcolor(LIGHTGRAY);
            return 1;
        }

        stringstream input_line_stream(input_line);
        input_line_stream >> num_players;
        char _ignore;

        if(input_line_stream.fail() || !(input_line_stream >> _ignore).eof()) {
            setcolor(RED);
            cout << endl << "Expected an integer." << endl;
            continue;
        }
    	
        if(num_players < 2) {
            setcolor(RED);
            cout << endl << "Must have at least 2 players." << endl;
            continue;
        }

        if(num_players > max_players) {
            setcolor(RED);
            cout << endl << "Must have at most " << max_players << " players." << endl;
            continue;
        }

        valid_num_players = true;
    }

    Game game(board, num_players, rng);
    GameDisplayer displayer(game);
    playGame(game, displayer, rng);
    
    displayer.drawGameOver();

    return 0;
}

int main() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    bool playAgain;
    do {
        if(playOnce(rng) == 1) return 1; // TODO
        cout << endl;
        playAgain = promptPlayAgain();
    } while(playAgain);

    return 0;
}
