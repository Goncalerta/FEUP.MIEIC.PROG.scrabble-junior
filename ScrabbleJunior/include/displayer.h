#ifndef DISPLAYER_H
#define DISPLAYER_H

class GameDisplayer;

#include <string>
#include <sstream>
#include <vector>
#include "game.h"
#include "board.h"
#include "position.h"
#include "cmd.h"
#include "player.h"
#include "hand.h"
#include "word.h"
#include "hand.h"

class GameDisplayer {
    static const Color PLAYERS_COLOR[];
    static const char* WINNER_LABELS[];

    std::ostringstream error_messages;
    Game &game;

    void drawBoard(const Board &board, const Hand &hand);
    void drawBoard(const Board &board, std::vector<Position> &legal_moves);
    void drawPlayers(const std::vector<Player> &players, int x_offset);
    void drawLeaderboard(std::vector<const Player*> players, int x_offset);
    void declareWinners(std::vector<const Player*> leaderboard);
    void drawCurrentPlayer();
    void drawErrorMessages();

    public:
    GameDisplayer(Game &game);

    std::ostream& getErrorStream();
    void clearErrors();

    void draw();
    void draw(std::vector<Position> &legal_moves);
    void drawUnplayable();
    void drawGameOver();

    Hand::SwapLetterAnimator animateRefillHand();
    Hand::SwapLetterAnimator animateExchange(char letter);
    Hand::SwapLetterAnimator animateExchange(char letter1, char letter2);
    void drawEmptyPoolWhenRefilling();
    void noticeDepletedPool();

    void animateWordComplete(const Word &word);
    void drawWordComplete(const std::vector<Word> &words_completed);

    static void drawBoard(const Board &board);
    void delay(int milliseconds);
};

#endif