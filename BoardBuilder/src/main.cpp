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

void checkDictionaryExistence() {
    while(!ifstream(BoardBuilder::DICTIONARY).is_open()) {
        setcolor(BoardBuilderDisplayer::ERROR_COLOR);
        cout << "Dictionary file '" << BoardBuilder::DICTIONARY 
                << "' was not found in the current folder or could not be opened." << endl
                << "Without this file, you can't fill the boards with words, as they can't be validated." << endl;
        setcolor(BoardBuilderDisplayer::TEXT_COLOR);
        cout << "Make sure such file exists and can be opened, then press ENTER: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool isValidCharForName(char c) {
    return isalnum(c) || c == '_';
}

bool promptBoardName(string &board_name) {
    while(true) {
        setcolor(BoardBuilderDisplayer::TEXT_COLOR);
        cout << "Name of new board: ";
        getline(cin, board_name);
        if(cin.fail()) return 0;

        if(board_name.size() == 0) {
            setcolor(BoardBuilderDisplayer::ERROR_COLOR);
            cout << "Must input a name." << endl;
            continue;
        }

        auto begin = board_name.begin();
        auto end = board_name.end();

        auto invalid_char = find_if_not(begin, end, isValidCharForName);
        if(invalid_char != end) {
            setcolor(BoardBuilderDisplayer::ERROR_COLOR);
            
            cout << "Only ASCII alfanumeric letters and underscores are allowed in board name." << endl;
            if(isspace(*invalid_char)) cout << "Whitespace is not allowed." << endl;
            else cout << "'" << *invalid_char << "' is not allowed." << endl;

            continue;
        }

        cout << "Board will be saved in file '" << board_name << ".txt'." << endl;

        return true;
    }
}

bool promptSize(int &size, const char *prompt, const char *dimension) {
    while(true) {
        setcolor(BoardBuilderDisplayer::TEXT_COLOR);
        cout << prompt;
        string input;
        getline(cin, input);
        if(cin.fail()) return false;

        stringstream input_stream(input);
        input_stream >> size;
        

        if(input_stream.fail() || size <= 0) {
            setcolor(BoardBuilderDisplayer::ERROR_COLOR);
            cout << "Expected a positive integer." << endl;
            continue;
        }

        std::string unexpected;
        input_stream >> unexpected;
        if(unexpected.size() != 0) {
            setcolor(BoardBuilderDisplayer::ERROR_COLOR);
            cout << "Unexpected '" << unexpected << "'." << endl;
            continue;
        }

        if(size > 20) {
            setcolor(BoardBuilderDisplayer::ERROR_COLOR);
            cout << dimension << " can only be at most 20." << endl;
            continue;
        } 

        return true;
    }
}

bool askLoadFile() {
    string input;
    while(true) {
        setcolor(BoardBuilderDisplayer::TEXT_COLOR);
        cout << "Do you wish to edit existing board? (Y/N): ";
        getline(cin, input);
        if(cin.fail()) return false;

        char answer;
        stringstream input_stream(input);
        input_stream >> answer;

        if(input_stream.fail()) {
            setcolor(BoardBuilderDisplayer::ERROR_COLOR);
            cout << "Invalid input." << endl;

        } else if(answer == 'Y' || answer == 'y') return true;
        else if(answer == 'N' || answer == 'n') return false;
        else {
            setcolor(BoardBuilderDisplayer::ERROR_COLOR);
            cout << "Invalid input." << endl;

        }
    }
}

int main() {
    checkDictionaryExistence();

    string board_name;
    int board_width, board_height;

    if(!promptBoardName(board_name)) return 0;

    bool will_overwrite_file = false;
    bool is_file_loadable = false;

    ifstream board_file(board_name + ".txt");
    if(board_file.is_open()) {
        will_overwrite_file = true;

        setcolor(BoardBuilderDisplayer::WARNING_COLOR);
        cout << "File '" << board_name << ".txt' already exists." << endl;

        if(askLoadFile()) {
            char _x; // Ignored delimiter character between height and width

            board_file >> board_height >> _x >> board_width;            

            if(board_file.fail() || board_height == 0 || board_height > 20 || board_width == 0 || board_width > 20) {
                setcolor(BoardBuilderDisplayer::ERROR_COLOR);
                cout << "Board could not be loaded. Contents of the file may have been corrupted." << endl;
            } else {
                is_file_loadable = true;
            }
        }
    }

    if(!is_file_loadable) {
        if(will_overwrite_file) {
            setcolor(BoardBuilderDisplayer::WARNING_COLOR);
            cout << "File '" << board_name << ".txt' will be overwritten when saving the board." << endl;
        }
        if(!promptSize(board_width, "Width (number of columns): ", "Width")) return 0;
        if(!promptSize(board_height, "Height (number of rows): ", "Height")) return 0;
    }

    Board board(board_width, board_height);
    if(is_file_loadable) board.loadWords(board_file);
    BoardBuilder builder(board_name, board);

    builder.run();
    builder.saveToFile();

    return 0;
}
