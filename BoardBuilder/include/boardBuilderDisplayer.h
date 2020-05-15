#ifndef BOARD_BUILDER_DISPLAYER_H
#define BOARD_BUILDER_DISPLAYER_H

#include <string>
#include <sstream>
#include "cmd.h"
#include "board.h"

class BoardBuilderDisplayer {
    static const Color TEXT_COLOR;
    static const Color TEXT_COLOR_DARK;
    static const Color ERROR_COLOR;
    static const Color WARNING_COLOR;
    static const Color SUCCESS_COLOR;
    static const Color LETTER_COLOR;
    static const Color BOARD_BACKGROUND;

    static const int ADD_LETTER_TO_BOARD_DELAY;

    std::ostringstream error_messages;
    unsigned int board_info_x_offset;
    unsigned int prompt_y_offset;

    public:
    BoardBuilderDisplayer(Board &board);

    void printBoard() const;
    void printBoardInfo() const;
    void printPrompt() const;
    void printNewWord(const Word &word) const;
};

#endif