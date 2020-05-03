#include "displayer.h"
#include "cmd.h"

using namespace std;

GameDisplayer::GameDisplayer(Game &game): 
    game(game), 
    error_messages(0)
{}

void GameDisplayer::pushError(char *error) {
    error_messages.push_back(string(error));
}

void GameDisplayer::clearErrors() {
    error_messages.clear();
}

void GameDisplayer::draw() {
    clrscr();
    drawBoard(game.getBoard(), game.getCurrentPlayer());
    drawPlayers(game.getPlayers());
    drawCurrentPlayer();
    drawErrorMessages();
    setcolor(LIGHTGRAY);
    cout << "Input a valid position in the board to play: ";
}

void GameDisplayer::drawBoard(const Board &board, const Player &current_player) {
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

void GameDisplayer::drawPlayers(const vector<Player> &players) {
    int i = 1;
    for(auto &player: players) {
        cout << "P" << i << " hand:" << endl;
        for(auto i = player.handBegin(); i <= player.handEnd(); i++) {
            cout << *i << " ";
        }
        cout << endl; 

        i++;
    }
}

void GameDisplayer::drawCurrentPlayer() {
    // TODO
    Player player = game.getCurrentPlayer();
    cout << "Player ? is playing this turn" << endl;
    cout << "Your hand: ";
    for(auto i = player.handBegin(); i <= player.handEnd(); i++) {
        cout << *i << " ";
    }
    cout << endl << endl;
}

void GameDisplayer::drawErrorMessages() {
    setcolor(RED);
    for(auto &error: error_messages) {
        cout << error << endl;
    }
}
