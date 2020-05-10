#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include "board.h"
#include "pool.h"
#include "orientation.h"
#include "position.h"
#include "player.h"
#include "game.h"
#include "displayer.h"
#include "cmd.h"
#include <algorithm>
#include <limits>

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
    if(!board_file.is_open() ) {
        if(file_name.find_first_of("/\\") == string::npos) {

            file_name += ".txt";
            board_file.open(file_name, ios_base::in);
            
            if(!board_file.is_open()) {
                setcolor(RED);
                cout << "File does not exist or is unavailable." << endl;
                return false;
            }

        } else {
            setcolor(RED);
            cout << "File does not exist or is unavailable." << endl;
            return false;
        }
    }
    
    return true;
}

bool loadBoardFile(Board &board, istream &board_file) {
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

        if(!board.addWord(Word(position, orientation, word))) return false; // TODO ERROR MESSAGE
    }

    return true;
}

bool playGame(Game &game, GameDisplayer &displayer, default_random_engine rng) {
    string p_input;

    do {
        const Player &player = game.getCurrentPlayer();

        if(game.getBoard().hasMove(player.getHand())) {
            vector<Position> edge_case_legal_positions;
            if(game.mustPlayTwiceEdgeCase(edge_case_legal_positions)) {
                vector<Position> &legal_positions = edge_case_legal_positions;
                auto is_legal = [legal_positions](Position pos, auto _) {
                    return find(legal_positions.begin(), legal_positions.end(), pos) != legal_positions.end();
                };
                displayer.draw(game.getBoard(), game.getPlayers(), game.getCurrentPlayer(), game.getMovesLeftThisTurn(), is_legal);
                cout << "Input a valid position on the board to play (in the form 'Ab'): ";
                if(getline(cin, p_input).fail()) return false;
                displayer.clearErrors();

                if(p_input.size() > 2) {
                    displayer.getErrorStream() << "Too many characters in input.\n";
                    continue;
                }

                if(p_input.size() < 2) {
                    displayer.getErrorStream() << "Too few characters in input.\n";
                    continue;
                }

                if(!Position::isValid(p_input[1], p_input[0])) {
                    displayer.getErrorStream() << "Couldn't parse input as a position.\n";
                    continue;
                }

                Position pos(p_input[1], p_input[0]);
                game.move(pos, displayer, edge_case_legal_positions); // TODO remove bool?
                if(game.getMovesLeftThisTurn() == 0) {
                    game.nextTurn(displayer);
                }
            } else {
                const Hand &hand = game.getCurrentPlayer().getHand();
                auto is_legal = [hand](auto _, const Cell &cell) {
                    return cell.isCoverable() && hand.hasLetter(cell.getLetter());
                };
                displayer.draw(game.getBoard(), game.getPlayers(), game.getCurrentPlayer(), game.getMovesLeftThisTurn(), is_legal);
                cout << "Input a valid position on the board to play (in the form 'Ab'): ";
                if(getline(cin, p_input).fail()) return false;
                displayer.clearErrors();

                if(p_input.size() > 2) {
                    displayer.getErrorStream() << "Too many characters in input.\n";
                    continue;
                }

                if(p_input.size() < 2) {
                    displayer.getErrorStream() << "Too few characters in input.\n";
                    continue;
                }

                if(!Position::isValid(p_input[1], p_input[0])) {
                    displayer.getErrorStream() << "Couldn't parse input as a position.\n";
                    continue;
                }

                Position pos(p_input[1], p_input[0]);
                game.move(pos, displayer); // TODO remove bool?
                if(game.getMovesLeftThisTurn() == 0) {
                    game.nextTurn(displayer);
                }
            }
        } else if(game.getMovesLeftThisTurn() == 1) {
            displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any more moves this turn.\n";

            displayer.draw(game.getBoard(), game.getPlayers(), game.getCurrentPlayer(), game.getMovesLeftThisTurn());
            displayer.clearErrors();
            cout << "Press ENTER to continue . . . " << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            game.nextTurn(displayer);
        } else if(game.getPool().size() >= 2) {
            displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any move.\n"
                    << "Must choose two letters to exchange with the Pool this turn.\n";

            displayer.draw(game.getBoard(), game.getPlayers(), game.getCurrentPlayer(), game.getMovesLeftThisTurn());
            cout << "Input two letters to exchange with the Pool: ";
            if(getline(cin, p_input).fail()) return false;
            displayer.clearErrors();

            char letter1, letter2;
            stringstream input_stream(p_input);
            input_stream >> letter1 >> letter2;
            char _ignore;

            if(input_stream.fail() || !(input_stream >> _ignore).eof()) {
                displayer.getErrorStream() << "Invalid input.\n";
                continue;
            }

            if(game.exchange(letter1, letter2, displayer, rng)) {
                game.nextTurn(displayer);
            }
        } else if(game.getPool().size() == 1) {
            displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any move.\n"
                    << "Must choose a letter this turn to exchange for the remaining one in the Pool.\n";

            displayer.draw(game.getBoard(), game.getPlayers(), game.getCurrentPlayer(), game.getMovesLeftThisTurn());
            cout << "Input a letter to exchange with the Pool: ";
            if(getline(cin, p_input).fail()) return false;
            displayer.clearErrors();

            char letter;
            stringstream input_stream(p_input);
            input_stream >> letter;
            char _ignore;

            if(input_stream.fail() || !(input_stream >> _ignore).eof()) {
                displayer.getErrorStream() << "Invalid input.\n";
                continue;
            }

            if(game.exchange(letter, displayer, rng)) {
                game.nextTurn(displayer);
            }
        } else {
            displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any move.\n"
                    << "Turn has been skipped.\n";

            displayer.draw(game.getBoard(), game.getPlayers(), game.getCurrentPlayer(), game.getMovesLeftThisTurn());
            displayer.clearErrors();
            cout << "Press ENTER to continue . . . " << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            game.nextTurn(displayer);
        }
    } while(!game.isOver());

    return true;
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

        if(input_stream.fail()) {
            setcolor(RED);
            cout << "Invalid input." << endl;
            setcolor(LIGHTGRAY);
        } else if(answer == 'Y' || answer == 'y') return true;
        else if(answer == 'N' || answer == 'n') return false;
        else {
            setcolor(RED);
            cout << "Invalid input." << endl;
            setcolor(LIGHTGRAY);
        }
    }
}

int playOnce(default_random_engine rng) {
    ifstream board_file;
    Board board;
    bool load_successful = false;

    while(!load_successful) {
        setcolor(LIGHTGRAY);
        if(!openBoardFile(board_file)) {
            if(cin.fail()) return 1;
            else continue;
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

    cout << endl;
    GameDisplayer::printBoard(board);
    cout << endl;

    if(max_players == 2) {
        cout << "This board only allows you to play a game with " << max_players << " players." << endl;
        cout << "Press ENTER to start a game with 2 players . . . " << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        num_players = 2;
        valid_num_players = true;
    }

    while(!valid_num_players) {
        
        setcolor(LIGHTGRAY);
        cout << "This board allows you to play a game with up to " << max_players << " players." << endl;
        cout << "Input the number of players (2-" << max_players << "): ";
        getline(cin, input_line);
        if(cin.fail()) return 1;

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

    clrscr();

    Game game(board, num_players, rng);
    GameDisplayer displayer(board.getWidth(), board.getHeight());
    if(!playGame(game, displayer, rng)) return 1;
    
    displayer.drawGameOver(board, game.getLeaderboard());

    return 0;
}

int main() {
    ios::sync_with_stdio(false);
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
