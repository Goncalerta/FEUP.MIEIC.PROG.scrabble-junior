#include <iostream>
#include <iomanip>
#include "displayer.h"
#include "cmd.h"

using namespace std;

GameDisplayer::GameDisplayer(Game &game): 
    game(game), 
    error_messages(0)
{}

void GameDisplayer::pushError(const char *error) {
    error_messages.push_back(string(error));
}

void GameDisplayer::clearErrors() {
    error_messages.clear();
}

void GameDisplayer::draw() {
    clrscr();
    drawBoard(game.getBoard(), game.getCurrentPlayer());
    int player_x_offset = game.getBoard().getWidth() * 2 + 2;
    drawPlayers(game.getPlayers(), player_x_offset);
    gotoxy(0, max(8, game.getBoard().getHeight()+2));
    drawCurrentPlayer();
    drawErrorMessages();
    setcolor(LIGHTGRAY);
}

void GameDisplayer::drawBoard(const Board &board, const Player &current_player) {
    // TODO only highlight when playable
    cout << ' ';
    setcolor(LIGHTGRAY);
    for(int i = 0; i < board.getWidth(); i++) {
        cout << (char)('a' + i) << ' ';
    }

    cout << endl;
    for(int j = 0; j < board.getHeight(); j++) {
        setcolor(LIGHTGRAY);
        cout << (char)('A' + j);
        
        for(int i = 0; i < board.getWidth(); i++) {
            const Cell cell = board.getCell(Position(i, j));
            
            if(cell.isEmpty()) {
                setcolor(BLACK, LIGHTGRAY);
                cout << ' ';
            } else {
                if(cell.isCovered()) setcolor(RED, LIGHTGRAY);
                else if(cell.isCoverable()) setcolor(BLACK, YELLOW);
                else setcolor(BLACK, LIGHTGRAY);
                
                cout << cell.getLetter();
            }

            if(i+1 != board.getWidth()) {
                setcolor(BLACK, LIGHTGRAY);
                cout << ' ';
            }
        }
        cout << endl;
    }
    setcolor(LIGHTGRAY);
}

void GameDisplayer::drawPlayers(const vector<Player> &players, int x_offset) {
    gotoxy(x_offset, 1);
    cout << "   score:       LETTERS   ";

    for(int i = 0; i < players.size(); i++) {
        gotoxy(x_offset, i+2);
        cout << "P" << i << " " << setw(4) << players[i].getScore() << "      ";
        for(auto letter = players[i].handBegin(); letter <= players[i].handEnd(); letter++) {
            if(*letter == Player::EMPTY_HAND) cout << '_' << " ";
            else cout << *letter << " ";
        }
    }
}

void GameDisplayer::drawCurrentPlayer() {
    Player player = game.getCurrentPlayer();
    cout << "Player " << game.getCurrentPlayerNumber() << " is playing this turn." << endl;
    if(game.getMovesLeftThisTurn() == 1) {
        cout << "You have 1 move left this turn." << endl;
    } else {
        cout << "You have " << game.getMovesLeftThisTurn() << " moves left this turn." << endl;
    }
    
    cout << "Your hand:  ";
    for(auto i = player.handBegin(); i <= player.handEnd(); i++) {
        if(*i == Player::EMPTY_HAND) cout << '_' << " ";
        else cout << *i << " ";
    }
    cout << endl << endl;
}

void GameDisplayer::drawErrorMessages() {
    setcolor(RED);
    for(auto &error: error_messages) {
        cout << error << endl;
    }
}
