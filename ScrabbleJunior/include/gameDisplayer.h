#ifndef GAME_DISPLAYER_H
#define GAME_DISPLAYER_H

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
     
    static const int WORD_COMPLETED_ANIMATION_DELAY;
    static const int SCORE_INCREASE_DELAY;
    static const int SWAP_LETTER_DELAY;
    static const int NOTICE_DELAY;
    static const int LONG_NOTICE_DELAY;
    static const int AFTER_REFILL_DELAY;

    static const unsigned int current_player_hand_x_offset;

    std::ostringstream error_messages;
    const unsigned int scoreboard_x_offset;
    const unsigned int turn_info_y_offset;

    static void printColoredId(int id, const char *prefix = "");

    static void printWord(const Word &word, bool delay_each_letter);

    public:
    static const Color TEXT_COLOR;
    static const Color ERROR_COLOR;
    static const Color WARNING_COLOR;
    static const Color SWAP_LETTER_COLOR;
    static const Color LETTER_UNCOVERED_COLOR;
    static const Color LETTER_COVERED_COLOR;
    static const Color SCORE_COLOR;
    static const Color BOARD_BACKGROUND;
    static const Color BOARD_HIGHLIGHTED_BACKGROUND;
    
    typedef std::function<bool (Position, Cell)> CheckLegalMove;

    GameDisplayer(unsigned int board_width, unsigned int board_height); 

    std::ostream& getErrorStream();
    void clearErrors();

    static void printBoard(const Board &board, CheckLegalMove check_legal_move = nullptr);
    void printScoreboard(const std::vector<Player> &players) const;
    void printTurnInfo(const Player &current_player, unsigned int moves_left) const;
    void printLeaderboard(const std::vector<Player> &players) const;
    void declareWinners(const std::vector<int> &winners_id, int num_players) const;

    Hand::SwapLetterAnimator getSwapLetterCallback() const;
    void animateWordComplete(const Player &player, const std::vector<Word> &words_completed) const;

    void notice(const std::string &information, bool long_delay = false) const;
    void afterRefill(bool depleted_pool) const;
};

#endif