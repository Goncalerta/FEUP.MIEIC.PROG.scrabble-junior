#include <string>
#include "game.h"

using namespace std;

Game::Game(int num_players):
    turn(0),
    players(num_players),
    state(Unstarted)
{}

Player& Game::getCurrentPlayer() {
    return players[turn%players.size()];
}

bool Game::loadBoardFile(std::istream &board_file) {
    if(state != Unstarted) return false;
    // string line;
    // getline(board_file, line);
    
    // if(board_file.fail()) return false;
    
    // while(getline(board_file, line)) {

    // }
    int width, height;
    char x;
    board_file >> height >> x >> width;

    if(!board_file || x != 'x') return false; 

    if(!board.setWidth(width)) return false;
    if(!board.setHeight(height)) return false;

    char c_position[2];
    char c_orientation;
    string word;
    while(board_file >> c_position >> c_orientation >> word) {
        Position position(c_position[1], c_position[0]);
        Orientation orientation;
        
        if(c_orientation == 'H') orientation = Horizontal;
        else if(c_orientation == 'V') orientation = Vertical;
        else return false;

        if(!board.addWord(position, orientation, word)) return false;

        pool.pushWord(word);
    }

    return board_file.eof();
}

void Game::startGame(default_random_engine &rng) {
    pool.shuffle(rng);
    
    for(auto &player: players) {
        player.refillHand(pool);
    }
}

const std::vector<Player>& Game::getPlayers() const {
    return players;
}

const Board& Game::getBoard() const {
    return board;
}

bool Game::move(Position position, GameDisplayer &displayer) {
    Player &player = getCurrentPlayer();
    if(!position.inRect(Position(0, 0), board.getWidth(), board.getHeight())) {
        displayer.pushError("Position is outside board limits.");
        return false;
    }
    char l = board.getLetter(position);

    if(!player.hasLetter(l)) {
        displayer.pushError("Doesn't have letter.");
        return false;
    }

    int score_gain = board.cover(position);
    if(score_gain == Board::ILLEGAL_MOVE) {
        displayer.pushError("Illegal move.");
        return false;
    }
    player.useLetter(l);
    player.addScore(score_gain);

    return true;
}

void Game::nextTurn() {
    getCurrentPlayer().refillHand(pool);
    turn++;
}
