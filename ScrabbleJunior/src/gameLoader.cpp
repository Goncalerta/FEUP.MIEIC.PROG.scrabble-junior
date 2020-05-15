#include <iostream>
#include "gameLoader.h"
#include "gameDisplayer.h"

using namespace std;

const Color GameLoader::TEXT_COLOR = GameDisplayer::TEXT_COLOR;
const Color GameLoader::ERROR_COLOR = GameDisplayer::ERROR_COLOR;

GameLoader::GameLoader():
    max_players(0),
    state(Unloaded)
{}

LoadState GameLoader::getState() const {
    return state;
}

void GameLoader::loadBoard(Board &board) {
    ifstream board_file;

    if(openBoardFile(board_file)) {
        if(loadBoardFile(board, board_file)) {
            if(board.countLetters() >= 14) {
                max_players = min(4u, board.countLetters()/7);

                cout << endl;
                GameDisplayer::printBoard(board);
                cout << endl;

                state = BoardLoaded;
            } else {
                setcolor(ERROR_COLOR);
                cout << "Board must have at least 14 letters in order to be playable." << endl;
            }
        } else {
            setcolor(ERROR_COLOR);
            cout << "Unable to load board." << endl;
        }

        board_file.close();
    }
}

bool GameLoader::openBoardFile(ifstream &board_file) {
    string file_name;

    setcolor(TEXT_COLOR);
    cout << "Input the name of your board: ";
    getline(cin, file_name);
    if(cin.fail()) {
        state = FailedToLoad;
        return false;
    }

    if(file_name.size() == 0) {
        setcolor(ERROR_COLOR);
        cout << "Must input a file name." << endl;
        return false;
    }

    board_file.open(file_name, ios_base::in);
    if(!board_file.is_open() ) {
        if(file_name.find_first_of("/\\") == string::npos) {

            file_name += ".txt";
            board_file.open(file_name, ios_base::in);
            
            if(!board_file.is_open()) {
                setcolor(ERROR_COLOR);
                cout << "File does not exist or is unavailable." << endl;
                return false;
            }

        } else {
            setcolor(ERROR_COLOR);
            cout << "File does not exist or is unavailable." << endl;
            return false;
        }
    }
    
    return true;
}

bool GameLoader::loadBoardFile(Board &board, ifstream &board_file) {
    board = Board();
    unsigned int width, height;

    board_file >> height;
    if(board_file.fail()) {
        setcolor(ERROR_COLOR);
        cout << "Failed to parse height in given file." << endl;
        return false;
    }
    if(height == 0) {
        setcolor(ERROR_COLOR);
        cout << "Height must not be zero." << endl;
        return false;
    }
    if(height > 20) {
        setcolor(ERROR_COLOR);
        cout << "Max height is 20." << endl;
        return false;
    }

    char _x; // Ignored
    board_file >> _x;

    board_file >> width;
    if(board_file.fail()) {
        setcolor(ERROR_COLOR);
        cout << "Failed to parse width in given file." << endl;
        return false;
    }
    if(width == 0) {
        setcolor(ERROR_COLOR);
        cout << "Width must not be zero." << endl;
        return false;
    }
    if(width > 20) {
        setcolor(ERROR_COLOR);
        cout << "Max width is 20." << endl;
        return false;
    }

    board.setSize(width, height);

    char c_position[2];
    char c_orientation;
    string word;

    Board board(width, height);

    char x, y, orientation_char;
    string word_str;

    while(board_file >> y >> x >> orientation_char >> word_str) {
        if(x < 'a' || x > 'z' || y < 'A' || y > 'Z') break;
        Position position(x, y);
        
        Orientation orientation;
        if(orientation_char == 'H') orientation = Horizontal;
        else if(orientation_char == 'V') orientation = Vertical;
        else break;

        Word word(position, orientation, word_str);

        board.addWord(word);
    }

    return true;
}

void GameLoader::promptNumberPlayers(int &num_players) {
    setcolor(TEXT_COLOR);
    if(max_players == 2) {
        cout << "This board only allows you to play a game with " << max_players << " players." << endl;
        cout << "Press ENTER to start a game with 2 players . . . " << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        num_players = 2;
        state = ReadyToPlay;
    } else {
        string input_line;

        cout << "This board allows you to play a game with up to " << max_players << " players." << endl;
        cout << "Input the number of players (2-" << max_players << "): ";
        getline(cin, input_line);
        if(cin.fail()) {
            state = FailedToLoad;
            return;
        }

        stringstream input_line_stream(input_line);
        input_line_stream >> num_players;
        char _ignore;

        if(input_line_stream.fail() || !(input_line_stream >> _ignore).eof()) {
            setcolor(ERROR_COLOR);
            cout << endl << "Expected an integer." << endl;

        } else if(num_players < 2) {
            setcolor(ERROR_COLOR);
            cout << endl << "Must have at least 2 players." << endl;

        } else if(num_players > max_players) {
            setcolor(ERROR_COLOR);
            cout << endl << "Must have at most " << max_players << " players." << endl;

        } else {
            state = ReadyToPlay;
        }
    }
}


