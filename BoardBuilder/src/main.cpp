#include <iostream>
#include <fstream>
#include <string>
#include <ios>
#include <limits>
#include "board.h"
#include "word.h"
#include "command.h"

using namespace std;

<<<<<<< HEAD
void promptWidth(Board& board) {
    int width;
    while (true) {
=======
void promptWidth(Board &board) {
    int width;
    while(true) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        cout << "Width (number of columns): ";
        // TODO should the line be read all at once? (in other words, unallow inputs such as "10p") 
        cin >> width;

<<<<<<< HEAD
        if (cin.fail()) {
=======
        if(cin.fail()) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            // if(cin.eof()) TODO
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "The given input is invalid." << endl;
            continue;
<<<<<<< HEAD
        }

        if (board.setWidth(width)) break;
=======
        } 
        
        if(board.setWidth(width)) break;
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        else cout << "The given input is invalid." << endl;
    }
}

// TODO reduce code duplication
<<<<<<< HEAD
void promptHeight(Board& board) {
    int height;
    while (true) {
=======
void promptHeight(Board &board) {
    int height;
    while(true) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
        cout << "Height (number of rows): ";
        // TODO should the line be read all at once? (in other words, unallow inputs such as "10p") 
        cin >> height;

<<<<<<< HEAD
        if (cin.fail()) {
=======
        if(cin.fail()) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            // if(cin.eof()) TODO
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "The given input is invalid." << endl;
            continue;
<<<<<<< HEAD
        }

        if (board.setHeight(height)) break;
=======
        } 
        
        if(board.setHeight(height)) break;
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
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
<<<<<<< HEAD
    if (cin.fail()) return 0;
=======
    if(cin.fail()) return 0;
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
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
<<<<<<< HEAD

        // Validate and parse input
        if (cin.fail()) {
            if (cin.eof()) break;
=======
        
        // Validate and parse input
        if(cin.fail()) {       
            if(cin.eof()) break;     
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "The given input is invalid." << endl;
            continue;
<<<<<<< HEAD
        }

        Command command(cx, cy, corientation, cword);
        if (!command.isValid()) {
=======
        } 

        Command command(cx, cy, corientation, cword);
        if(!command.isValid()) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            cout << "The given input is invalid." << endl;
            continue;
        }

        Word word(command);
<<<<<<< HEAD
        if (!word.inDict()) {
=======
        if(!word.inDict()) {
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
            cout << "The given word wasn't found in the dictionary." << endl;
            continue;
        }

<<<<<<< HEAD
        if (!board.addWord(word)) {
            cout << "The given word doesn't fit the board or collides with incompatible words at given position." << endl;
            continue;
        }
    } while (!cin.eof());
=======
        if(!board.addWord(word)) {
            cout << "The given word doesn't fit the board or collides with incompatible words at given position." << endl;
            continue;
        }
    } while(!cin.eof());
>>>>>>> af2634ed998871d93345b34770acad66809fd03f

    ofstream outfile(boardname + ".txt");
    board.printData(outfile);

    return 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> af2634ed998871d93345b34770acad66809fd03f
