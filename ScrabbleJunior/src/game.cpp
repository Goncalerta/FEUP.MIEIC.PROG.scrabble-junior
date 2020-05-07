#include <string>
#include <sstream>
#include <algorithm>
#include "game.h"

using namespace std;

Game::Game(Board &board, unsigned int num_players, std::default_random_engine &rng):
    board(board),
    pool(board.getLettersInBoard()),
    turn(0),
    moves_left(2)
{
    pool.shuffle(rng);
    
    for(int i = 1; i <= num_players; i++) {
        Player player(i);
        player.refillHand(pool);
        players.push_back(player);
    }
}

Player& Game::getCurrentPlayer() {
    return players[turn % players.size()];
}

int Game::getCurrentPlayerNumber() const {
    return (turn % players.size()) + 1;
}

int Game::getMovesLeftThisTurn() const {
    return moves_left;
}

int Game::getMovesThisTurn() const {
    return 2 - moves_left;
}

const std::vector<Player>& Game::getPlayers() const {
    return players;
}

const Board& Game::getBoard() const {
    return board;
}

const Pool& Game::getPool() const {
    return pool;
}

bool Game::isOver() const {
    return board.isFullyCovered();
}

vector<const Player*> Game::getLeaderboard() const {
    vector<const Player*> leaderboard;
    for(auto &player: players) {
        leaderboard.push_back(&player);
    }

    stable_sort(leaderboard.begin(), leaderboard.end(), 
            [](auto p1, auto p2){ return p1->getScore() > p2->getScore(); });
            
    return leaderboard;
}

int Game::getLeadingScorePlayerNumber() const {
    // TODO doesn't take into account draws
    return max_element(players.begin(), players.end(), [](auto p1, auto p2){return p1.getScore() < p2.getScore();}) - players.begin() + 1;
}

bool Game::move(Position position, GameDisplayer &displayer) {
    Player &player = getCurrentPlayer();
    if(!position.inRect(Position(0, 0), board.getWidth(), board.getHeight())) {
        displayer.pushError("Position is outside board limits.");
        return false;
    }
    char l = board.getLetter(position);
    const Board &cboard = board;

    if(cboard.getCell(position).isEmpty()) {
        displayer.pushError("The given position has no letter.");
        return false;
    }

    if(cboard.getCell(position).isCovered()) {
        displayer.pushError("That position has already been covered.");
        return false;
    }

    if(!cboard.getCell(position).isCoverable()) {
        displayer.pushError("Can't move to that position.");
        return false;
    }

    if(!player.hasLetter(l)) {
        stringstream error;
        error << "You don't have letter '" << l << "' in your hand.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    int score_gain = board.cover(position);
    player.useLetter(l);
    player.addScore(score_gain);
    moves_left -= 1;

    return true;
}

// TODO reduce duplication
bool Game::move(Position position, GameDisplayer &displayer, std::vector<Position> &legal_moves) {
    Player &player = getCurrentPlayer();
    if(!position.inRect(Position(0, 0), board.getWidth(), board.getHeight())) {
        displayer.pushError("Position is outside board limits.");
        return false;
    }
    char l = board.getLetter(position);
    const Board &cboard = board;

    if(cboard.getCell(position).isEmpty()) {
        displayer.pushError("The given position has no letter.");
        return false;
    }

    if(cboard.getCell(position).isCovered()) {
        displayer.pushError("That position has already been covered.");
        return false;
    }

    if(!cboard.getCell(position).isCoverable()) {
        displayer.pushError("Can't move to that position.");
        return false;
    }

    if(!player.hasLetter(l)) {
        stringstream error;
        error << "You don't have letter '" << l << "' in your hand.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    bool is_legal = any_of(legal_moves.begin(), legal_moves.end(), [position](Position pos){ return pos == position;});
    if(!is_legal) {
        displayer.pushError("There is at least one move that allows you to play twice this turn. This move would only allow you to play once.");
        return false;
    }

    int score_gain = board.cover(position);
    player.useLetter(l);
    player.addScore(score_gain);
    moves_left -= 1;

    return true;
}

bool Game::exchange(char letter, GameDisplayer &displayer, default_random_engine &rng) {
    Player &player = getCurrentPlayer();
    letter = toupper(letter);
    if(!Player::isValidLetter(letter)) {
        stringstream error;
        error << "Character '" << letter << "' is not a letter.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    if(!player.hasLetter(letter)) {
        stringstream error;
        error << "You don't have letter '" << letter << "' in your hand.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    auto animator = displayer.animateExchange(letter);
    player.exchange(pool, letter, animator);

    pool.shuffle(rng);
    return true;
}

bool Game::exchange(char letter1, char letter2, GameDisplayer &displayer, default_random_engine &rng) {
    Player &player = getCurrentPlayer();
    letter1 = toupper(letter1);
    if(!Player::isValidLetter(letter1)) {
        stringstream error;
        error << "Character '" << letter1 << "' is not a letter.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    letter2 = toupper(letter2);
    if(!Player::isValidLetter(letter2)) {
        stringstream error;
        error << "Character '" << letter2 << "' is not a letter.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    bool has_letters = true;

    if(!player.hasLetter(letter1)) {
        stringstream error;
        error << "You don't have letter '" << letter1 << "' in your hand.";
        displayer.pushError(error.str().c_str());
        has_letters = false;
    }

    if(!player.hasLetter(letter2)) {
        stringstream error;
        error << "You don't have letter '" << letter2 << "' in your hand.";
        displayer.pushError(error.str().c_str());
        has_letters = false;
    }

    if(!has_letters) return false;

    auto animator = displayer.animateExchange(letter1, letter2);
    player.exchange(pool, letter1, letter2, animator);

    pool.shuffle(rng);
    return true;
}

void Game::nextTurn(GameDisplayer &displayer) {
    Player &current = getCurrentPlayer();
    if(current.needsRefill()) {
        if(pool.isEmpty()) displayer.drawEmptyPoolWhenRefilling();
        else {
            auto animator = displayer.animateRefillHand();
            current.refillHand(pool, animator);

            if(pool.isEmpty()) displayer.noticeDepletedPool();
            else displayer.delay(750);
        }
    } 
    
    moves_left = 2;
    turn++;
}

bool Game::canCurrentPlayerMove() {
    Player &current = getCurrentPlayer();
    return board.hasMove(current.handBegin(), current.handEnd());
}

bool Game::mustPlayTwiceEdgeCase(std::vector<Position> &positions) {
    if(getMovesThisTurn() > 0) return false;
    Player &current_player = getCurrentPlayer();
    return board.mustPlayTwiceEdgeCase(positions, current_player.handBegin(), current_player.handEnd());
}
