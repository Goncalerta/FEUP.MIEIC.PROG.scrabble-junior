#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <string>
#include <sstream>
#include <vector>
#include "board.h"
#include "position.h"
#include "cmd.h"
#include "player.h"
#include "hand.h"
#include "word.h"
#include "hand.h"
#include "cell.h"

class GameDisplayer {
    static const Color PLAYERS_COLOR[];
    static const char* WINNER_LABELS[];

    static const Color TEXT_COLOR;
    static const Color ERROR_COLOR;
    static const Color WARNING_COLOR;
    static const Color SWAP_LETTER_COLOR;
    static const Color LETTER_UNCOVERED_COLOR;
    static const Color LETTER_COVERED_COLOR;
    static const Color SCORE_COLOR;
    static const Color BOARD_BACKGROUND;
    static const Color BOARD_HIGHLIGHTED_BACKGROUND;
     
    static const int WORD_COMPLETED_ANIMATION_DELAY;
    static const int SCORE_INCREASE_DELAY;
    static const int SWAP_LETTER_DELAY;
    static const int NOTICE_DELAY;
    static const int NOTICE_BEFORE_SWAP_DELAY;
    static const int AFTER_REFILL_DELAY;

    static const unsigned int current_player_hand_x_offset;

    std::ostringstream error_messages;
    unsigned int scoreboard_x_offset;
    unsigned int turn_info_y_offset;

    static void GameDisplayer::printWord(const Word &word, bool delay_each_letter);

    static std::vector<int> getWinnersId(const std::vector<const Player*> &leaderboard); // TODO does this belong here?
    static void printColoredId(int id);

    public:
    typedef std::function<bool (Position, Cell)> CheckLegalMove;

    GameDisplayer(unsigned int board_width, unsigned int board_height); 

    std::ostream& getErrorStream();
    void clearErrors();

    static void printBoard(const Board &board, CheckLegalMove check_legal_move = nullptr); // Can I make it so that it doesn't have to be static?

    // TODO Those two don't seem to belong in this class
    void draw(const Board &board, const std::vector<Player> &players, const Player &current_player, int moves_left, CheckLegalMove check_legal_move = nullptr) const;
    void drawGameOver(const Board &board, const std::vector<const Player*> &leaderboard) const;

    void printScoreboard(const std::vector<Player> &players) const;
    void printLeaderboard(const std::vector<const Player*> &players) const;

    // TODO maybe those could be just one function
    void noticeDepletedPool() const;
    void noticeEmptyPool() const;


    void printTurnInfo(const Player &current_player, unsigned int moves_left) const;
    void animateWordComplete(const Player &player, const std::vector<Word> &words_completed) const;
    void declareWinners(const std::vector<const Player*> &leaderboard) const;

    Hand::SwapLetterAnimator getSwapLetterCallback() const;
    // TODO all of those could be joined with notice functions
    void animateRefillHand();
    void animateExchange(char letter);
    void animateExchange(char letter1, char letter2);

    void clearTurnInfo() const;

    void delayAfterRefill() const;
};

#endif