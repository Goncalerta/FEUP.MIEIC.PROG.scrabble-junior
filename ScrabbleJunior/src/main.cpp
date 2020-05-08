#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include "board.h"
#include "pool.h"
#include "orientation.h"
#include "position.h"
#include "player.h"

using namespace std;

bool parseBoardFile(istream &board_file, Board &board, Pool &pool) {
     //string line;
     //getline(board_file, line);
    
     //if(board_file.fail()) return false;
    
     //while(getline(board_file, line)) {

     //}
    int width, height;
    char x;
    board_file >> height >> x >> width;

    if(!board_file || x != 'x') return false; 

    if(!board.setWidth(width)) return false;
    if(!board.setHeight(height)) return false;

    char c_position[2];
    char c_orientation;
    string word;
    while(board_file >> c_position >> c_orientation >> word) {
        Position position(c_position[1], c_position[0]);
        Orientation orientation;
        
        if(c_orientation == 'H') orientation = Horizontal;
        else if(c_orientation == 'V') orientation = Vertical;
        else return false;

        if(!board.addWord(position, orientation, word)) return false;

        pool.pushWord(word);
    }

    return board_file.eof();
}

int main() {
    srand(time(0));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    // TODO ask number of players
    // TODO ask name of the board file 
    string board_file_name = "TEST.txt";
    ifstream board_file(board_file_name);

    Board board;
    Pool pool;

    if(!parseBoardFile(board_file, board, pool)) {
        cout << "Invalid board file";
        return 1; // TODO 
    }

    pool.shuffle(rng);

    Player p1;
    Player p2;
    int turn = 0;

    p1.refillHand(pool);
    p2.refillHand(pool);

    board.printGrid(cout);
    cout << "P1 hand:" << endl;
    for(auto i = p1.handBegin(); i <= p1.handEnd(); i++) {
        cout << *i << " ";
    }    
    cout << endl;
    cout << "P2 hand:" << endl;
    for(auto i = p2.handBegin(); i <= p2.handEnd(); i++) {
        cout << *i << " ";
    }    
    cout << endl;

    string p_input;
    while(getline(cin, p_input)) {
        if(p_input.size() != 2) {
            cout << "Too many chars in input";
            continue;
        }

        Position pos(p_input[1], p_input[0]);
        if(turn % 2 == 0) {
            char l = board.getLetter(pos);
            if(!p1.hasLetter(l)) {
                cout << "Doesn't have letter.";
                continue;
            }
            int score_gain = board.cover(pos);
            if(score_gain == Board::ILLEGAL_MOVE) {
                cout << "Illegal move.";
                continue;
            }
            p1.useLetter(l);
            p1.addScore(score_gain);
        } else {

        }
        turn++;
    }

    return 0;
}


// // PROG - MIEIC
// // JAS
// // Example of a program that prints colored characters on the console (in text mode)
// #include <iostream>
// #include <ctime>
// #include <cstdlib>
// #include <windows.h>
// //==========================================================================================
// //COLOR CODES: (alternative: use symbolic const’s)
// #define BLACK 0
// #define BLUE 1
// #define GREEN 2
// #define CYAN 3
// #define RED 4
// #define MAGENTA 5
// #define BROWN 6
// #define LIGHTGRAY 7
// #define DARKGRAY 8
// #define LIGHTBLUE 9
// #define LIGHTGREEN 10
// #define LIGHTCYAN 11
// #define LIGHTRED 12
// #define LIGHTMAGENTA 13
// #define YELLOW 14
// #define WHITE 15
// //==========================================================================================
// void clrscr(void)
// {
//  COORD coordScreen = { 0, 0 }; // upper left corner
//  DWORD cCharsWritten;
//  DWORD dwConSize;
//  HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
//  CONSOLE_SCREEN_BUFFER_INFO csbi;
//  GetConsoleScreenBufferInfo(hCon, &csbi);
//  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
//  // fill with spaces
//  FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
//  GetConsoleScreenBufferInfo(hCon, &csbi);
//  FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
//  // cursor to upper left corner
//  SetConsoleCursorPosition(hCon, coordScreen);
// }
// //==========================================================================================
// // Position the cursor at column 'x', line 'y'
// // The coodinates of upper left corner of the screen are (x,y)=(0,0)
// void gotoxy(int x, int y)
// {
//  COORD coord;
//  coord.X = x;
//  coord.Y = y;
//  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
// }
// //==========================================================================================
// // Set text color
// void setcolor(unsigned int color)
// {
//  HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
//  SetConsoleTextAttribute(hcon, color);
// }
// //==========================================================================================
// // Set text color & background
// void setcolor(unsigned int color, unsigned int background_color)
// {
//  HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
//  if (background_color == BLACK)
//  SetConsoleTextAttribute(hCon, color);
//  else
//  SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
//  BACKGROUND_RED);
// }
// //==========================================================================================
// // Fill the screen with colored numbers
// int main()
// {
//  clrscr();
//  srand((unsigned int)time(NULL));
//  for (int x = 0; x < 80; x++)
//  for (int y = 0; y < 24; y++)
//  {
//  gotoxy(x, y);
//  if (rand() % 2 == 0)
//  setcolor(x % 15 + 1);
//  else
//  setcolor(y % 15 + 1, rand() % 2);
//  cout << x % 10;
//  }
// }