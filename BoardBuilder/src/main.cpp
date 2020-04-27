#include <iostream>
#include <fstream>
#include <string>
#include <ios>
#include <limits>
#include "board.h"
#include "word.h"
#include "command.h"

using namespace std;

void promptWidth(Board &board) {
    int width;
    while(true) {
        cout << "Width (number of columns): ";
        // TODO should the line be read all at once? (in other words, unallow inputs such as "10p") 
        cin >> width;

        if(cin.fail()) {
            // if(cin.eof()) TODO
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "The given input is invalid." << endl;
            continue;
        } 
        
        if(board.setWidth(width)) break;
        else cout << "The given input is invalid." << endl;
    }
}

// TODO reduce code duplication
void promptHeight(Board &board) {
    int height;
    while(true) {
        cout << "Height (number of rows): ";
        // TODO should the line be read all at once? (in other words, unallow inputs such as "10p") 
        cin >> height;

        if(cin.fail()) {
            // if(cin.eof()) TODO
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "The given input is invalid." << endl;
            continue;
        } 
        
        if(board.setHeight(height)) break;
        else cout << "The given input is invalid." << endl;
    }
}

int main() {
    // TODO check whether dictionary is present.
    string boardname;
    Board board;

    int width;
    cout << "Name of new board: ";
    cin >> boardname;
    if(cin.fail()) return 0;
    // TODO check if it is a valid name (is it really necessary?)

    promptWidth(board);
    promptHeight(board);

    do {
        cout << endl;
        board.printGrid(cout);
        cout << endl;

        cout << "Enter a word (\"Aa H|V WORD\") or use Ctrl+Z to submit file: ";
        char cx, cy, corientation;
        string cword;
        // TODO should the line be read all at once? (in other words, unallow inputs such as "Aa\nH\nword") 
        cin >> cy >> cx >> corientation >> cword;
        
        // Validate and parse input
        if(cin.fail()) {       
            if(cin.eof()) break;     
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "The given input is invalid." << endl;
            continue;
        } 

        Command command(cx, cy, corientation, cword);
        if(!command.isValid()) {
            cout << "The given input is invalid." << endl;
            continue;
        }

        Word word(command);
        if(!word.inDict()) {
            cout << "The given word wasn't found in the dictionary." << endl;
            continue;
        }

        if(!board.addWord(word)) {
            cout << "The given word doesn't fit the board or collides with incompatible words at given position." << endl;
            continue;
        }
    } while(!cin.eof());

    ofstream outfile(boardname + ".txt");
    board.printData(outfile);

    return 0;
}
