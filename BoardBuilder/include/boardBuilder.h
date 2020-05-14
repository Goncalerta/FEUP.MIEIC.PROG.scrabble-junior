#ifndef BOARD_BUILDER_H
#define BOARD_BUILDER_H

#include <string>
#include <sstream>
#include "cmd.h"
#include "board.h"

class BoardBuilder {
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
    std::string board_name;
    Board &board;
    unsigned int max_players;

    void printBoardInfo() const;
    void printNewWord(const Word &word) const;

    public:
    static const char* DICTIONARY;
    
    static bool inDict(std::istream &dict, std::string word);
    BoardBuilder(std::string &board_name, Board &board);

    bool loop();
    void printBoard() const;
    void saveToFile() const;
};

#endif