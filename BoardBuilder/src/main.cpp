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
const Color WARNING_COLOR = YELLOW;

bool promptSize(int &size, char *prompt, char *dimension) {
    while(true) {
        setcolor(TEXT_COLOR);
        cout << prompt;
        string input_line;
        getline(cin, input_line);
        if(cin.fail()) return false;

        stringstream input_line_stream(input_line);
        input_line_stream >> size;
        

        if(input_line_stream.fail() || size <= 0) {
            setcolor(ERROR_COLOR);
            cout << "Expected a positive integer." << endl;
            continue;
        }

        std::string unexpected;
        input_line_stream >> unexpected;
        if(unexpected.size() != 0) {
            setcolor(ERROR_COLOR);
            cout << "Unexpected '" << unexpected << "'." << endl;
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

        cout << "Board will be saved in file '" << board_name << ".txt'." << endl;

        return true;
    }
}

bool promptEditExisting() {
    string input;
    while(true) {
        cout << "Do you wish to edit existing board? (Y/N): ";
        getline(cin, input);
        if(cin.fail()) return false;

        char answer;
        stringstream input_stream(input);
        input_stream >> answer;

        if(input_stream.fail()) {
            setcolor(ERROR_COLOR);
            cout << "Invalid input." << endl;
            setcolor(TEXT_COLOR);
        } else if(answer == 'Y' || answer == 'y') return true;
        else if(answer == 'N' || answer == 'n') return false;
        else {
            setcolor(ERROR_COLOR);
            cout << "Invalid input." << endl;
            setcolor(TEXT_COLOR);
        }
    }
}

int main() {
    while(!ifstream(BoardBuilder::DICTIONARY).is_open()) {
        setcolor(ERROR_COLOR);
        cout << "Dictionary file '" << BoardBuilder::DICTIONARY 
                << "' was not found in the current folder or could not be opened." << endl
                << "Without this file, you can't fill the boards with words, as they can't be validated." << endl;
        setcolor(TEXT_COLOR);
        cout << "Make sure such file exists and can be opened, then press ENTER: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string board_name;
    int width, height;

    if(!promptBoardName(board_name)) return 0;

    ifstream board_file(board_name + ".txt");
    if(board_file.is_open()) {
        setcolor(WARNING_COLOR);
        cout << "File '" << board_name << ".txt" << "' already exists." << endl;

        if(promptEditExisting()) {
            char _x; // Ignored delimiter character between height and width

            board_file >> height >> _x >> width;            

            if(!(board_file.fail() || height == 0 || height > 20 || width == 0 || width > 20)) {
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

                    if(!board.isWordValid(word)) break;
                    board.addWord(word);
                }

                BoardBuilder builder(board_name, board);

                clrscr();
                builder.printBoard();

                do {} while(builder.loop());
                
                cin.clear();
                cout << endl;
                builder.saveToFile();
            } else {
                setcolor(ERROR_COLOR);
                cout << "Board could not be loaded. Contents of the file may have been corrupted." << endl;
                setcolor(WARNING_COLOR);
                cout << "File '" << board_name << ".txt' will be overwritten when saving the board." << endl;
                if(!promptSize(width, "Width (number of columns): ", "Width")) return 0;
                if(!promptSize(height, "Height (number of rows): ", "Height")) return 0;

                Board board(width, height);
                BoardBuilder builder(board_name, board);

                clrscr();
                builder.printBoard();

                do {} while(builder.loop());
                
                cin.clear();
                cout << endl;
                builder.saveToFile();
            }
        } else {
            setcolor(WARNING_COLOR);
            cout << "File '" << board_name << ".txt' will be overwritten when saving the board." << endl;
            if(!promptSize(width, "Width (number of columns): ", "Width")) return 0;
            if(!promptSize(height, "Height (number of rows): ", "Height")) return 0;

            Board board(width, height);
            BoardBuilder builder(board_name, board);

            clrscr();
            builder.printBoard();

            do {} while(builder.loop());
            
            cin.clear();
            cout << endl;
            builder.saveToFile();
        }

    } else {
        if(!promptSize(width, "Width (number of columns): ", "Width")) return 0;
        if(!promptSize(height, "Height (number of rows): ", "Height")) return 0;

        Board board(width, height);
        BoardBuilder builder(board_name, board);

        clrscr();
        builder.printBoard();

        do {} while(builder.loop());
        
        cin.clear();
        cout << endl;
        builder.saveToFile();
    }

    return 0;
}
