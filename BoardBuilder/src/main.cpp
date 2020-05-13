#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ios>
#include <limits>
#include <cctype>
#include <algorithm>
#include "board.h"
#include "word.h"
#include "boardBuilder.h"
#include "cmd.h"

using namespace std;

const Color TEXT_COLOR = LIGHTGRAY;
const Color ERROR_COLOR = RED;

bool promptSize(int &size, char *prompt, char *dimension) {
    while(true) {
        setcolor(TEXT_COLOR);
        cout << prompt;
        string input_line;
        getline(cin, input_line);
        if(cin.fail()) return false;

        stringstream input_line_stream(input_line);
        input_line_stream >> size;
        char _ignore;

        if(input_line_stream.fail() || !(input_line_stream >> _ignore).eof() || size <= 0) {
            setcolor(ERROR_COLOR);
            cout << "Expected a positive integer." << endl;
            continue;
        }

        if(size > 20) {
            setcolor(ERROR_COLOR);
            cout << dimension << " can only be at most 20." << endl;
            continue;
        } 

        return true;
    }
}

bool isValidCharForName(char c) {
    return isalnum(c) || c == '_';
}

bool promptBoardName(string &board_name) {
    while(true) {
        setcolor(TEXT_COLOR);
        cout << "Name of new board: ";
        getline(cin, board_name);
        if(cin.fail()) return 0;

        if(board_name.size() == 0) {
            setcolor(ERROR_COLOR);
            cout << "Must input a name." << endl;
            continue;
        }

        auto begin = board_name.begin();
        auto end = board_name.end();

        auto invalid_char = find_if_not(begin, end, isValidCharForName);
        if(invalid_char != end) {
            setcolor(ERROR_COLOR);
            
            cout << "Only ASCII alfanumeric letters and underscores are allowed in board name." << endl;
            if(isspace(*invalid_char)) cout << "Whitespace is not allowed." << endl;
            else cout << "'" << *invalid_char << "' is not allowed." << endl;

            continue;
        }

        cout << "Board will be saved as '" << board_name << ".txt'." << endl;

        return true;
    }
}

int main() {
    // TODO check whether dictionary is present.
    string board_name;
    int width, height;

    if(!promptBoardName(board_name)) return 0;
    if(!promptSize(width, "Width (number of columns): ", "Width")) return 0;
    if(!promptSize(height, "Height (number of rows): ", "Height")) return 0;

    Board board(width, height);
    BoardBuilder builder(board_name, board);

    clrscr();
    builder.printBoard();


    do {
        
    } while(builder.loop());

    

    return 0;
}
