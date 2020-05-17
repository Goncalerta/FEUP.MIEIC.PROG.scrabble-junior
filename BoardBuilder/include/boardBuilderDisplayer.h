#ifndef BOARD_BUILDER_DISPLAYER_H
#define BOARD_BUILDER_DISPLAYER_H

#include <string>
#include <sstream>
#include <ostream>
#include "cmd.h"
#include "board.h"

class BoardBuilderDisplayer {
    static const int ADD_LETTER_TO_BOARD_DELAY;

    std::ostringstream error_messages;
    unsigned int board_info_x_offset;
    unsigned int prompt_y_offset;

    public:
    static const Color TEXT_COLOR;
    static const Color TEXT_COLOR_DARK;
    static const Color ERROR_COLOR;
    static const Color WARNING_COLOR;
    static const Color SUCCESS_COLOR;
    static const Color LETTER_COLOR;
    static const Color BOARD_BACKGROUND;

    BoardBuilderDisplayer(unsigned int board_width, unsigned int board_height);

    std::ostream& getErrorStream();
    void clearErrors();

    void printBoard(const Board &board) const;
    void printBoardInfo(const std::string &board_name, const Board &board, unsigned int max_players) const;
    void printPrompt(unsigned int max_players, unsigned int total_letters) const;
    void printNewWord(const Word &word, const Board &board) const;
};

#endif