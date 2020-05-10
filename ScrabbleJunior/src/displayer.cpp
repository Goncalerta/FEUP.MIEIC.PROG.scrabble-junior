#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "displayer.h"

using namespace std;

const Color GameDisplayer::PLAYERS_COLOR[] = {CYAN, LIGHTRED, GREEN, YELLOW};
const char* GameDisplayer::WINNER_LABELS[] = {"1st", "2nd", "3rd", "4th"};

const Color GameDisplayer::TEXT_COLOR = LIGHTGRAY;
const Color GameDisplayer::ERROR_COLOR = RED;
const Color GameDisplayer::WARNING_COLOR = YELLOW;
const Color GameDisplayer::SWAP_LETTER_COLOR = YELLOW;
const Color GameDisplayer::LETTER_UNCOVERED_COLOR = BLACK;
const Color GameDisplayer::LETTER_COVERED_COLOR = RED;
const Color GameDisplayer::SCORE_COLOR = GREEN;
const Color GameDisplayer::BOARD_BACKGROUND = LIGHTGRAY;
const Color GameDisplayer::BOARD_HIGHLIGHTED_BACKGROUND = YELLOW;

const int GameDisplayer::WORD_COMPLETED_ANIMATION_DELAY = 200;
const int GameDisplayer::SCORE_INCREASE_DELAY = 500;
const int GameDisplayer::SWAP_LETTER_DELAY = 400;
const int GameDisplayer::NOTICE_DELAY = 1500;
const int GameDisplayer::NOTICE_BEFORE_SWAP_DELAY = 800;
const int GameDisplayer::AFTER_REFILL_DELAY = 750;

const unsigned int  GameDisplayer::current_player_hand_x_offset = 12;

GameDisplayer::GameDisplayer(unsigned int board_width, unsigned int board_height):
    scoreboard_x_offset(board_width*2 + 2),
    turn_info_y_offset(max(8u, board_height + 2))
{}

std::ostream& GameDisplayer::getErrorStream() {
    return error_messages;
}

void GameDisplayer::clearErrors() {
    error_messages.str("");
}

void GameDisplayer::printBoard(const Board &board, CheckLegalMove check_legal_move) {
    cout << ' ';
    for(char l = 'a'; l < board.getWidth() + 'a'; l++) {
        cout << l << ' ';
    }
    cout << '\n';

    for(int j = 0; j < board.getHeight(); j++) {
        char l = j + 'A';
        cout << l;
        
        for(int i = 0; i < board.getWidth(); i++) {
            Position position(i, j);
            const Cell cell = board.getCell(position);
            char letter = cell.getLetter();
            Color letter_color, letter_background;

            if(cell.isCovered()) {
                letter_color = LETTER_COVERED_COLOR;
            } else {
                letter_color = LETTER_UNCOVERED_COLOR;
            }

            if(check_legal_move && check_legal_move(position, cell)) {
                letter_background = BOARD_HIGHLIGHTED_BACKGROUND;
            } else {
                letter_background = BOARD_BACKGROUND;
            }

            setcolor(letter_color, letter_background);
            if(cell.isEmpty()) cout << ' ';
            else cout << letter;
            
            if(i+1 != board.getWidth()) {
                setcolor(BLACK, BOARD_BACKGROUND);
                cout << ' ';
            }
        }
        setcolor(TEXT_COLOR);
        cout << '\n';
    }
}

void GameDisplayer::printScoreboard(const std::vector<Player> &players) const {
    gotoxy(scoreboard_x_offset + 4, 1);
    cout << "SCORE       LETTERS";

    for(int i = 0; i < players.size(); i++) {
        gotoxy(scoreboard_x_offset, i+2);
        unsigned int id = players[i].getId();
        unsigned int score = players[i].getScore();
        const Hand &hand = players[i].getHand();

        setcolor(PLAYERS_COLOR[id-1]);
        cout << "P" << id;
        setcolor(TEXT_COLOR);
        cout << " " << setw(4) << score << "      " << hand;
    }
}

void GameDisplayer::printLeaderboard(const std::vector<const Player*> &players) const {
    gotoxy(scoreboard_x_offset + 9, 1);
    cout << "SCORE";
    // A negative value is guaranteed to be different from any score.
    // Needed so first player will always show a 'WINNER_LABEL'.
    int previous_score = -1; 

    for(int i = 0; i < players.size(); i++) {
        gotoxy(scoreboard_x_offset, i+2);
        unsigned int id = players[i]->getId();
        unsigned int score = players[i]->getScore();
        
        if(score != previous_score) cout << WINNER_LABELS[i] << "  ";
        else cout << "     ";
        
        setcolor(PLAYERS_COLOR[id-1]);
        cout << "P" << id;
        setcolor(TEXT_COLOR);
        cout << " " << setw(4) << score;
        
        previous_score = score;
    }
}

void GameDisplayer::printWord(const Word &word, bool delay_each_letter) {
    int x = word.getStart().getX()*2 + 1;
    int y = word.getStart().getY() + 1;

    for(const char &c: word) {
        gotoxy(x, y);
        cout << c;

        if(delay_each_letter) { 
            this_thread::sleep_for(chrono::milliseconds(WORD_COMPLETED_ANIMATION_DELAY));
        }

        if(word.getOrientation() == Horizontal) x += 2;
        else y += 1;
    }
}

void GameDisplayer::noticeDepletedPool() const {
    gotoxy(0, turn_info_y_offset+6);
    setcolor(WARNING_COLOR);
    cout << "The pool has been depleted.";
    setcolor(TEXT_COLOR);
    this_thread::sleep_for(chrono::milliseconds(NOTICE_DELAY));
}

void GameDisplayer::noticeEmptyPool() const {
    gotoxy(0, turn_info_y_offset+4);
    setcolor(WARNING_COLOR);
    cout << "The Pool is empty . . .";
    setcolor(TEXT_COLOR);

    this_thread::sleep_for(chrono::milliseconds(NOTICE_DELAY));
}

std::vector<int> GameDisplayer::getWinnersId(const std::vector<const Player*> &leaderboard) {
    int winner_score = leaderboard[0]->getScore();
    vector<int> winners_id;

    for(const Player *player: leaderboard) {
        int score = player->getScore();
        int id = player->getId();
        
        if(score == winner_score) winners_id.push_back(id);
        else break;
    }

    return winners_id;
}

void GameDisplayer::printColoredId(int id) {
    setcolor(PLAYERS_COLOR[id-1]);
    cout << id;
    setcolor(TEXT_COLOR);
}

void GameDisplayer::printTurnInfo(const Player &current_player, unsigned int moves_left) const {
    gotoxy(0, turn_info_y_offset);
    unsigned int id = current_player.getId();

    setcolor(PLAYERS_COLOR[id-1]);
    cout << "Player " << id;
    setcolor(TEXT_COLOR);
    cout << " is playing this turn." << endl;

    if(moves_left == 0) {
        cout << "No moves left this turn." << endl;
    } else if(moves_left == 1) {
        cout << "You have 1 move left this turn." << endl;
    } else {
        cout << "You have " << moves_left << " moves left this turn." << endl;
    }
    
    cout << "Your hand:  " << current_player.getHand() << endl << endl;
}

void GameDisplayer::animateWordComplete(const Player &player, const vector<Word> &words_completed) const {
    gotoxy(0, turn_info_y_offset);
    setcolor(SCORE_COLOR);
    cout << "Score!";

    int score = player.getScore();
    int id = player.getId();

    for(const Word &word: words_completed) {
        setcolor(SCORE_COLOR, BOARD_BACKGROUND);
        printWord(word, true);
        setcolor(LETTER_COVERED_COLOR, BOARD_BACKGROUND);
        printWord(word, false);

        score+=1; 
        gotoxy(scoreboard_x_offset+3, id+1);
        setcolor(SCORE_COLOR);
        cout << setw(4) << score;
        
        this_thread::sleep_for(chrono::milliseconds(SCORE_INCREASE_DELAY));
    }

    setcolor(TEXT_COLOR);
}

void GameDisplayer::declareWinners(const vector<const Player*> &leaderboard) const {
    gotoxy(0, turn_info_y_offset);
    cout << "GAME OVER" << endl;

    vector<int> winners_id = getWinnersId(leaderboard);

    if(winners_id.size() == 1) {
        int id = winners_id[0];
        setcolor(PLAYERS_COLOR[id-1]);
        cout << "Player " << id << " wins.";
        setcolor(TEXT_COLOR);
    } else if(winners_id.size() == leaderboard.size()) {
        if(winners_id.size() == 2) cout << "Both ";
        else cout << "All ";

        cout << "players tied for first place.";
    } else {
        cout << "Players ";
        if(winners_id.size() == 3) {
            printColoredId(winners_id[0]);
            cout << ", ";
            printColoredId(winners_id[1]);
            cout << " and "; 
            printColoredId(winners_id[2]);
        } else {
            printColoredId(winners_id[0]);
            cout << " and "; 
            printColoredId(winners_id[1]);
        }
        
        cout << " tied for first place.";
    }

    cout << endl;
}

void GameDisplayer::clearTurnInfo() const {
    clrscr(0, turn_info_y_offset);
}

void GameDisplayer::draw(const Board &board, const vector<Player> &players, const Player &current_player, int moves_left, CheckLegalMove check_legal_move) const {
    clearTurnInfo(); // TODO maybe doesn't belong here
    
    // const Hand &hand = current_player.getHand();
    // auto is_legal_move = [hand](auto _, const Cell &cell) {
    //     return cell.isCoverable() && hand.hasLetter(cell.getLetter());
    // };
    
    gotoxy(0, 0);
    printBoard(board, check_legal_move);
    printScoreboard(players);
    printTurnInfo(current_player, moves_left);
    cout << error_messages.str();
}

void GameDisplayer::drawGameOver(const Board &board, const vector<const Player*> &leaderboard) const {
    clrscr();
    printBoard(board);
    printLeaderboard(leaderboard);
    declareWinners(leaderboard);
}

Hand::SwapLetterAnimator GameDisplayer::getSwapLetterCallback() const {
    return [this](int index, char letter) {
        setcolor(SWAP_LETTER_COLOR);
        gotoxy(current_player_hand_x_offset + 2*index, turn_info_y_offset+2);
        cout << letter;
        setcolor(TEXT_COLOR);

        this_thread::sleep_for(chrono::milliseconds(SWAP_LETTER_DELAY));
    };
}

void GameDisplayer::animateRefillHand() {
    // drawUnplayable();
    setcolor(WARNING_COLOR);
    cout << "Refilling hand . . .";
    setcolor(TEXT_COLOR);

    this_thread::sleep_for(chrono::milliseconds(NOTICE_BEFORE_SWAP_DELAY));
}

void GameDisplayer::animateExchange(char letter) {
    // drawUnplayable();
    setcolor(WARNING_COLOR);
    cout << "Exchanging letter '" << letter << "' . . .";
    setcolor(TEXT_COLOR);

    this_thread::sleep_for(chrono::milliseconds(NOTICE_BEFORE_SWAP_DELAY));
}

void GameDisplayer::animateExchange(char letter1, char letter2) {
    // drawUnplayable(); 
    setcolor(WARNING_COLOR);
    cout << "Exchanging letters '" << letter1 << "' and '" << letter2 << "' . . .";
    setcolor(TEXT_COLOR);

    this_thread::sleep_for(chrono::milliseconds(NOTICE_BEFORE_SWAP_DELAY));
}

void GameDisplayer::delayAfterRefill() const {
    this_thread::sleep_for(chrono::milliseconds(AFTER_REFILL_DELAY));
}
