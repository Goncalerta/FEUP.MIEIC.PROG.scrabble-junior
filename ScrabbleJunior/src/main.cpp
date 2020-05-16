#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include "board.h"
#include "pool.h"
#include "orientation.h"
#include "position.h"
#include "player.h"
#include "game.h"
#include "gameDisplayer.h"
#include "cmd.h"
#include <algorithm>
#include <limits>

using namespace std;

const Color TEXT_COLOR = GameDisplayer::TEXT_COLOR;
const Color ERROR_COLOR = GameDisplayer::ERROR_COLOR;

bool isValidCharForBoardName(char c) {
    return isalnum(c) || c == '_';
}

bool openBoardFile(ifstream &board_file, string &file_name) {
    if(file_name.size() == 0) {
        setcolor(ERROR_COLOR);
        cout << "Must input a path, the name of a file or a board name." << endl;
        return false;
    }

    // If user inputs the name of the board instead
    // of the filename (or a path), extension must be
    // appended.
    auto begin = file_name.begin();
    auto end = file_name.end();
    auto first_invalid_char = find_if_not(begin, end, isValidCharForBoardName);
    if(first_invalid_char == end) {
        file_name += ".txt";
    }

    board_file.open(file_name, ios_base::in);
    if(!board_file.is_open() ) {
        setcolor(ERROR_COLOR);
        cout << "File does not exist or is unavailable." << endl;
        return false;
    }
    
    return true;
}

bool readBoardSize(unsigned int &width, unsigned int &height, ifstream &board_file) {
    board_file >> height;
    if(board_file.fail() || height == 0 || height > 20) {
        setcolor(ERROR_COLOR);
        cout << "Failed to parse height in given file." << endl;
        return false;
    }

    char _x; // Ignored
    board_file >> _x;

    board_file >> width;
    if(board_file.fail() || width == 0 || width > 20) {
        setcolor(ERROR_COLOR);
        cout << "Failed to parse width in given file." << endl;
        return false;
    }

    return true;
}

bool promptNumberPlayers(int &num_players, unsigned int board_letters) {
    unsigned int max_players = min(4u, board_letters/7);

    if(max_players == 2) {
        setcolor(TEXT_COLOR);
        cout << "This board only allows you to play a game with " << max_players << " players." << endl;
        cout << "Press ENTER to start a game with 2 players . . . " << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        num_players = 2;
        return true;
    }

    while(true) {
        string input_line;

        setcolor(TEXT_COLOR);
        cout << "This board allows you to play a game with up to " << max_players << " players." << endl;
        cout << "Input the number of players (2-" << max_players << "): ";
        getline(cin, input_line);
        if(cin.fail()) return false; 

        stringstream input_line_stream(input_line);
        input_line_stream >> num_players;
        char _ignore;

        if(input_line_stream.fail() || !(input_line_stream >> _ignore).eof()) {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << endl << "Expected a positive integer." << endl;
            continue;
        }
        
        if(num_players < 2) {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << endl << "Must have at least 2 players." << endl;
            continue;
        }
        
        if(num_players > max_players) {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << endl << "Must have at most " << max_players << " players." << endl;
            continue;
        }

        return true;
    }
}

bool promptPlayAgain() {
    string input;
    while(true) {
        setcolor(TEXT_COLOR);
        cout << endl << "Play again? (Y/N): ";
        getline(cin, input);
        if(cin.fail()) return false;

        char answer;
        stringstream input_stream(input);
        input_stream >> answer;

        if(input_stream.fail()) {
            setcolor(ERROR_COLOR);
            cout << "Invalid input." << endl;
        } else if(answer == 'Y' || answer == 'y') { 
            return true;
        } else if(answer == 'N' || answer == 'n') {
            return false;
        } else {
            setcolor(ERROR_COLOR);
            cout << "Invalid input." << endl;
        }
    }
}

int main() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    while(true) {        
        string file_name;

        setcolor(TEXT_COLOR);
        cout << "Input the name of your board: ";
        getline(cin, file_name);
        if(cin.fail()) return 0;
        
        ifstream board_file;
        if(!openBoardFile(board_file, file_name)) continue;

        unsigned int width, height;
        if(!readBoardSize(width, height, board_file)) {
            board_file.close();
            continue;
        }

        Board board(width, height);
        board.loadWords(board_file);
        board_file.close();

        if(board.countLetters() < 14) {
            setcolor(ERROR_COLOR);
            cout << "Board must have at least 14 letters in order to be playable." << endl;
            continue;
        }

        // Board was successfuly loaded at this point.
        // Now it's time to show current board and prompt the user
        // to choose the number of players.
        cout << endl;
        GameDisplayer::printBoard(board);
        cout << endl;

        int num_players; 
        promptNumberPlayers(num_players, board.countLetters());

        // Everything is ready to start the game
        Game game(board, num_players);
        
        bool game_ended_without_error = game.play(rng);
        if(!game_ended_without_error) return 0;

        if(!promptPlayAgain()) break;
    }

    return 0;
}
