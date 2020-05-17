#ifndef BOARD_BUILDER_H
#define BOARD_BUILDER_H

#include <string>
#include <sstream>
#include "board.h"
#include "boardBuilderDisplayer.h"
#include "position.h"
#include "orientation.h"

class BoardBuilder {
    std::string board_name;
    Board &board;
    BoardBuilderDisplayer displayer;
    unsigned int max_players;

    bool parsePosition(std::istream &input, Position &position);
    bool parseOrientation(std::istream &input, Orientation &orientation);
    bool parseWordStr(std::istream &input, std::string &word_str);
    static bool inDict(std::istream &dict, std::string word);

    public:
    static const char* DICTIONARY;

    BoardBuilder(std::string &board_name, Board &board);

    void run();
    void saveToFile() const;
};

#endif