#include <string>
#include <sstream>
#include <algorithm>
#include "game.h"

using namespace std;

Game::Game(Board &board, unsigned int num_players, default_random_engine &rng):
    board(board),
    pool(board.getLettersInBoard()),
    current_player_index(0),
    moves_left(2)
{
    pool.shuffle(rng);
    
    for(int i = 1; i <= num_players; i++) {
        Player player(i);
        player.getHand().refill(pool);
        players.push_back(player);
    }
}

const vector<Player>& Game::getPlayers() const {
    return players;
}

const Board& Game::getBoard() const {
    return board;
}

const Pool& Game::getPool() const {
    return pool;
}

const Player& Game::getCurrentPlayer() const {
    return players[current_player_index];
}

int Game::getMovesLeftThisTurn() const {
    return moves_left;
}

bool Game::isOver() const {
    return board.isFullyCovered();
}

vector<const Player*> Game::getLeaderboard() const {
    vector<const Player*> leaderboard;
    for(const Player &player: players) {
        leaderboard.push_back(&player);
    }

    stable_sort(leaderboard.begin(), leaderboard.end(), 
            [](auto p1, auto p2) { return p1->getScore() > p2->getScore(); });
            
    return leaderboard;
}

bool Game::validateMove(Position position, GameDisplayer &displayer) {
    Player &player = players[current_player_index];
    
    if(!position.inLimits(board.getWidth(), board.getHeight())) {
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

    if(!player.getHand().hasLetter(l)) {
        stringstream error;
        error << "You don't have letter '" << l << "' in your hand.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    return true;
}

void Game::_move(Position position, GameDisplayer &displayer) {
    Player &player = players[current_player_index];
    char l = board.getLetter(position);

    moves_left -= 1;
    player.getHand().useLetter(l);

    vector<Word> completed_words;
    board.cover(position, completed_words);

    if(completed_words.size() != 0) {
        displayer.drawWordComplete(completed_words);
    }
    
    player.addScore(completed_words.size());
}

bool Game::move(Position position, GameDisplayer &displayer) {
    if(!validateMove(position, displayer)) return false;
    _move(position, displayer);

    return true;
}

bool Game::move(Position position, GameDisplayer &displayer, vector<Position> &legal_moves) {
    if(!validateMove(position, displayer)) return false;
    
    bool is_legal = find(legal_moves.begin(), legal_moves.end(), position) != legal_moves.end();

    if(!is_legal) {
        displayer.pushError("There is at least one move that allows you to play twice this turn. This move would only allow you to play once.");
        return false;
    }
    
    _move(position, displayer);

    return true;
}

bool Game::exchange(char letter, GameDisplayer &displayer, default_random_engine &rng) {
    Player &player = players[current_player_index];
    letter = toupper(letter);
    if(letter < 'A' || letter > 'Z') {
        stringstream error;
        error << "Character '" << letter << "' is not a letter.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    if(!player.getHand().hasLetter(letter)) {
        stringstream error;
        error << "You don't have letter '" << letter << "' in your hand.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    auto animator = displayer.animateExchange(letter);
    player.getHand().exchange(pool, letter, animator);

    pool.shuffle(rng);
    return true;
}

bool Game::exchange(char letter1, char letter2, GameDisplayer &displayer, default_random_engine &rng) {
    Player &player = players[current_player_index];
    letter1 = toupper(letter1);
    if(letter1 < 'A' || letter1 > 'Z') {
        stringstream error;
        error << "Character '" << letter1 << "' is not a letter.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    letter2 = toupper(letter2);
    if(letter2 < 'A' || letter2 > 'Z') {
        stringstream error;
        error << "Character '" << letter2 << "' is not a letter.";
        displayer.pushError(error.str().c_str());
        return false;
    }

    bool has_letters = true;

    if(!player.getHand().hasLetter(letter1)) {
        stringstream error;
        error << "You don't have letter '" << letter1 << "' in your hand.";
        displayer.pushError(error.str().c_str());
        has_letters = false;
    }

    if(!player.getHand().hasLetter(letter2)) {
        stringstream error;
        error << "You don't have letter '" << letter2 << "' in your hand.";
        displayer.pushError(error.str().c_str());
        has_letters = false;
    }

    if(!has_letters) return false;

    auto animator = displayer.animateExchange(letter1, letter2);
    player.getHand().exchange(pool, letter1, letter2, animator);

    pool.shuffle(rng);
    return true;
}

void Game::nextTurn(GameDisplayer &displayer) {
    Player &player = players[current_player_index];
    if(!player.getHand().isFull()) {
        if(pool.isEmpty()) displayer.drawEmptyPoolWhenRefilling();
        else {
            auto animator = displayer.animateRefillHand();
            player.getHand().refill(pool, animator);

            if(pool.isEmpty()) displayer.noticeDepletedPool();
            else displayer.delay(750);
        }
    } 
    
    moves_left = 2;
    current_player_index++;
    if(current_player_index == players.size()) current_player_index = 0;
}

bool Game::mustPlayTwiceEdgeCase(vector<Position> &positions) {
    if(moves_left < 2) return false;
    Player &player = players[current_player_index];
    return board.mustPlayTwiceEdgeCase(positions, player.getHand());
}
