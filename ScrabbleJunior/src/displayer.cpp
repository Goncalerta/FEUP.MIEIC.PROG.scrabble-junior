#include <iostream>
#include <iomanip>
#include "displayer.h"
#include <chrono>
#include <thread>

using namespace std;

const Color GameDisplayer::PLAYERS_COLOR[] = {CYAN, LIGHTRED, GREEN, YELLOW};
const char* GameDisplayer::WINNER_LABELS[] = {"1st", "2nd", "3rd", "4th"};

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
    
    Player &current_player = game.getCurrentPlayer();
    drawBoard(game.getBoard(), current_player.getHand());
    
    int player_x_offset = game.getBoard().getWidth() * 2 + 2;
    drawPlayers(game.getPlayers(), player_x_offset);
    
    gotoxy(0, max(8, game.getBoard().getHeight()+2));
    drawCurrentPlayer();
    
    drawErrorMessages();
}

// TODO reduce code duplication
void GameDisplayer::draw(std::vector<Position> &legal_moves) {
    clrscr();
    
    Player &current_player = game.getCurrentPlayer();
    drawBoard(game.getBoard(), legal_moves);
    
    int player_x_offset = game.getBoard().getWidth() * 2 + 2;
    drawPlayers(game.getPlayers(), player_x_offset);
    
    gotoxy(0, max(8, game.getBoard().getHeight()+2));
    drawCurrentPlayer();
    
    drawErrorMessages();
}

void GameDisplayer::drawUnplayable() {
    clrscr();
    
    drawBoard(game.getBoard());
    
    int player_x_offset = game.getBoard().getWidth() * 2 + 2;
    drawPlayers(game.getPlayers(), player_x_offset);
    
    gotoxy(0, max(8, game.getBoard().getHeight()+2));
    drawCurrentPlayer();

    drawErrorMessages();
}

void GameDisplayer::drawGameOver() {
    clrscr();
    drawBoard(game.getBoard());
    
    int player_x_offset = game.getBoard().getWidth() * 2 + 2;
    vector<const Player*>leaderboard = game.getLeaderboard();
    drawLeaderboard(leaderboard, player_x_offset);

    gotoxy(0, max(8, game.getBoard().getHeight()+2));
    declareWinners(leaderboard);
}

void GameDisplayer::declareWinners(vector<const Player*> leaderboard) {
    int winner_score = leaderboard[0]->getScore();
    vector<int> winners_id;

    for(auto &player: leaderboard) {
        int score = player->getScore();
        int id = player->getId();
        
        if(score == winner_score) winners_id.push_back(id);
        else break;
    }

    cout << "GAME OVER" << endl;
    if(winners_id.size() == 1) {
        int id = winners_id[0];
        setcolor(PLAYERS_COLOR[id-1]);
        cout << "Player " << id << " wins.";
        setcolor(LIGHTGRAY);
    } else if(winners_id.size() == leaderboard.size()) {
        if(winners_id.size() == 2) cout << "Both ";
        else cout << "All ";
        cout << "players tied for first place.";
    } else {
        cout << "Players ";
        if(winners_id.size() == 3) {
            setcolor(PLAYERS_COLOR[winners_id[0]-1]);
            cout << winners_id[0];
            setcolor(LIGHTGRAY);
            cout << ", ";
            setcolor(PLAYERS_COLOR[winners_id[1]-1]);
            cout << winners_id[1];
            setcolor(LIGHTGRAY);
            cout << " and "; 
            setcolor(PLAYERS_COLOR[winners_id[2]-1]);
            cout << winners_id[2];
            setcolor(LIGHTGRAY);
        } else {
            setcolor(PLAYERS_COLOR[winners_id[0]-1]);
            cout << winners_id[0];
            setcolor(LIGHTGRAY);
            cout << " and "; 
            setcolor(PLAYERS_COLOR[winners_id[1]-1]);
            cout << winners_id[1];
            setcolor(LIGHTGRAY);
        }
        
        cout << " tied for first place.";
    }

    cout << endl;
}

void GameDisplayer::drawBoard(const Board &board, const Hand &hand) {
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
                else if(cell.canCover(hand)) setcolor(BLACK, YELLOW);
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

// TODO reduce code duplication
void GameDisplayer::drawBoard(const Board &board) {
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

// TODO reduce code duplication
void GameDisplayer::drawBoard(const Board &board, std::vector<Position> &legal_moves) {
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
            Position position(i, j);
            const Cell cell = board.getCell(position);
            
            if(cell.isEmpty()) {
                setcolor(BLACK, LIGHTGRAY);
                cout << ' ';
            } else {
                bool is_legal = any_of(legal_moves.begin(), legal_moves.end(), [position](Position pos){ return pos == position;});
                if(cell.isCovered()) setcolor(RED, LIGHTGRAY);
                else if(is_legal) setcolor(BLACK, YELLOW);
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
    cout << "    SCORE       LETTERS   ";

    for(int i = 0; i < players.size(); i++) {
        gotoxy(x_offset, i+2);

        int id = players[i].getId();
        setcolor(PLAYERS_COLOR[id-1]);
        cout << "P" << id;
        setcolor(LIGHTGRAY);
        cout << " " << setw(4) << players[i].getScore() << "      ";
        cout << players[i].getHand();
    }
}

// TODO reduce code duplication
void GameDisplayer::drawLeaderboard(std::vector<const Player*> players, int x_offset) {

    gotoxy(x_offset + 9, 1);
    cout << "SCORE";
    int previous_score = -1;

    for(int i = 0; i < players.size(); i++) {
        gotoxy(x_offset, i+2);
        int id = players[i]->getId();
        int score = players[i]->getScore();
        
        if(score != previous_score) cout << WINNER_LABELS[i] << "  ";
        else cout << "     ";
        
        setcolor(PLAYERS_COLOR[id-1]);
        cout << "P" << id;
        setcolor(LIGHTGRAY);
        cout << " " << setw(4) << score;
        
        previous_score = score;
    }
}

void GameDisplayer::drawCurrentPlayer() {
    Player player = game.getCurrentPlayer();
    setcolor(PLAYERS_COLOR[player.getId()-1]);
    cout << "Player " << player.getId();
    setcolor(LIGHTGRAY);
    cout << " is playing this turn." << endl;

    if(game.getMovesLeftThisTurn() == 0) {
        cout << "No moves left this turn." << endl;
    } else if(game.getMovesLeftThisTurn() == 1) {
        cout << "You have 1 move left this turn." << endl;
    } else {
        cout << "You have " << game.getMovesLeftThisTurn() << " moves left this turn." << endl;
    }
    
    cout << "Your hand:  " << player.getHand() << endl << endl;
}

void GameDisplayer::drawErrorMessages() {
    setcolor(RED);
    for(auto &error: error_messages) {
        cout << error << endl;
    }
    setcolor(LIGHTGRAY);
}

void GameDisplayer::noticeDepletedPool() {
    gotoxy(0, max(8, game.getBoard().getHeight()+2+6));
    setcolor(RED);
    cout << "The pool has been depleted.";
    setcolor(LIGHTGRAY);
    this_thread::sleep_for(chrono::milliseconds(1250));
}

Hand::SwapLetterAnimator GameDisplayer::animateRefillHand() {
    drawUnplayable();
    setcolor(YELLOW);
    cout << "Refilling hand . . .";
    setcolor(LIGHTGRAY);

    this_thread::sleep_for(chrono::milliseconds(800));

    int x = 12;
    int y = game.getBoard().getHeight()+2;

    return [x, y](int i, char c) { 
        setcolor(YELLOW);
        gotoxy(x + 2*i, max(8, y)+2);
        cout << c;
        setcolor(LIGHTGRAY);

        this_thread::sleep_for(chrono::milliseconds(400));
    };
}

void GameDisplayer::drawEmptyPoolWhenRefilling() {
    drawUnplayable();
    setcolor(YELLOW);
    cout << "The Pool is empty . . .";
    setcolor(LIGHTGRAY);

    this_thread::sleep_for(chrono::milliseconds(1000));
}

Hand::SwapLetterAnimator GameDisplayer::animateExchange(char letter) {
    drawUnplayable();
    setcolor(YELLOW);
    cout << "Exchanging letter '" << letter << "' . . .";
    setcolor(LIGHTGRAY);

    this_thread::sleep_for(chrono::milliseconds(800));

    int x = 12;
    int y = game.getBoard().getHeight()+2;

    return [x, y](int i, char c) { 
        setcolor(YELLOW);
        gotoxy(x + 2*i, max(8, y)+2);
        cout << c;
        setcolor(LIGHTGRAY);
        this_thread::sleep_for(chrono::milliseconds(400));
    };
}

Hand::SwapLetterAnimator GameDisplayer::animateExchange(char letter1, char letter2) {
    drawUnplayable();
    setcolor(YELLOW);
    cout << "Exchanging letters '" << letter1 << "' and '" << letter2 << "' . . .";
    setcolor(LIGHTGRAY);

    this_thread::sleep_for(chrono::milliseconds(800));

    int x = 12;
    int y = game.getBoard().getHeight()+2;

    return [x, y](int i, char c) { 
        setcolor(YELLOW);
        gotoxy(x + 2*i, max(8, y)+2);
        cout << c;
        setcolor(LIGHTGRAY);

        this_thread::sleep_for(chrono::milliseconds(400));
    };
}

void GameDisplayer::delay(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void GameDisplayer::animateWordComplete(const Word &word) {
    const Board &board = game.getBoard();

    Position position = word.getStart();
    int x = position.getX()*2 + 1;
    int y = position.getY() + 1;
    setcolor(GREEN, LIGHTGRAY);
    for(const char &c: word) {
        gotoxy(x, y);
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(200));
        if(word.getOrientation() == Horizontal) x += 2;
        else y += 1;
    }

    x = position.getX()*2 + 1;
    y = position.getY() + 1;
    setcolor(RED, LIGHTGRAY);
    for(const char &c: word) {
        gotoxy(x, y);
        cout << c;
        if(word.getOrientation() == Horizontal) x += 2;
        else y += 1;
    }
    setcolor(LIGHTGRAY);
}

void GameDisplayer::drawWordComplete(vector<Word> &words_completed) {
    drawUnplayable();
    setcolor(GREEN);
    cout << "Score!";
    setcolor(LIGHTGRAY);
    const Player &current = game.getCurrentPlayer();
    int score = current.getScore();
    int id = current.getId();

    int player_x_offset = game.getBoard().getWidth() * 2 + 2;

    for(auto &word: words_completed) {
        animateWordComplete(word);

        score+=1; 
        gotoxy(player_x_offset+3, 1+id);
        setcolor(GREEN);
        cout << setw(4) << score;
        setcolor(LIGHTGRAY);

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}
