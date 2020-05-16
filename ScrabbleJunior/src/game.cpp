#include <iostream> // TODO get rid of this
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include "game.h"

using namespace std;

Game::Game(Board &board, unsigned int num_players):
    board(board),
    pool(board.getLettersInBoard()),
    current_player_index(0),
    moves_left(2),
    displayer(board.getWidth(), board.getHeight())
{
    for(int i = 1; i <= num_players; i++) {
        Player player(i);
        players.push_back(player);
    }
}

bool Game::play(default_random_engine &rng) {
    pool.shuffle(rng);
    
    for(Player &player: players) {
        player.getHand().refill(pool);
    }

    clrscr();
    while(!isOver()) {
        bool game_ended_without_error = playLoop(rng);
        if(!game_ended_without_error) return false;
    }
    displayer.drawGameOver(board, getLeaderboard());
    return true;
}

bool Game::playLoop(default_random_engine &rng) {
    string p_input;

    const Player &player = getCurrentPlayer();

    if(board.hasMove(player.getHand())) {
        vector<Position> edge_case_legal_positions;
        bool must_play_twice = mustPlayTwiceEdgeCase(edge_case_legal_positions);
        GameDisplayer::CheckLegalMove is_legal = nullptr;

        if(must_play_twice) {
            vector<Position> &legal_positions = edge_case_legal_positions;
            is_legal = [legal_positions](Position pos, auto _) {
                return find(legal_positions.begin(), legal_positions.end(), pos) != legal_positions.end();
            };
        } else {
            const Hand &hand = player.getHand();
            is_legal = [hand](auto _, const Cell &cell) {
                return cell.isCoverable() && hand.hasLetter(cell.getLetter());
            };
        }

        displayer.draw(board, players, player, moves_left, is_legal);
        displayer.clearErrors();
        setcolor(GameDisplayer::TEXT_COLOR);
        cout << "Input a valid position on the board to play (in the form 'Ab'): ";
        getline(cin, p_input);
        if(cin.fail()) return false;

        // TODO allow trailing whitespace: use "Expected" tecnique
        if(p_input.size() > 2) {
            displayer.getErrorStream() << "Too many characters in input.\n";
            return true;
        }

        if(p_input.size() < 2) {
            displayer.getErrorStream() << "Too few characters in input.\n";
            return true;
        }

        if(p_input[0] < 'a' || p_input[0] > 'z' || p_input[1] < 'A' || p_input[1] > 'Z') {
            displayer.getErrorStream() << "Couldn't parse input as a position.\n";
            return true;
        }

        Position pos(p_input[1], p_input[0]);
            
        if(must_play_twice) {
            move(pos, displayer, edge_case_legal_positions); // TODO remove bool?
        } else {
            move(pos, displayer); // TODO remove bool?
        }

        if(moves_left == 0) {
            nextTurn(displayer);
        }
    } else if(moves_left == 1) {
        displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any more moves this turn.\n";

        displayer.draw(board, players, player, moves_left);
        displayer.clearErrors();

        cout << "Press ENTER to continue . . . " << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        nextTurn(displayer);
    } else if(pool.size() >= 2) {
        displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any move.\n"
                << "Must choose two letters to exchange with the Pool this turn.\n";

        displayer.draw(board, players, player, moves_left);
        displayer.clearErrors();
        cout << "Input two letters to exchange with the Pool: ";
        getline(cin, p_input);
        if(cin.fail()) return false;
        
        char letter1, letter2;
        stringstream input_stream(p_input);
        input_stream >> letter1 >> letter2;
        char _ignore;

        if(input_stream.fail() || !(input_stream >> _ignore).eof()) {
            displayer.getErrorStream() << "Invalid input.\n";
            return true;
        }

        if(exchange(letter1, letter2, displayer, rng)) {
            nextTurn(displayer);
        }
    } else if(pool.size() == 1) {
        displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any move.\n"
                << "Must choose a letter this turn to exchange for the remaining one in the Pool.\n";

        displayer.draw(board, players, player, moves_left);
        displayer.clearErrors();
        cout << "Input a letter to exchange with the Pool: ";
        getline(cin, p_input);
        if(cin.fail()) return false;

        char letter;
        stringstream input_stream(p_input);
        input_stream >> letter;
        char _ignore;

        if(input_stream.fail() || !(input_stream >> _ignore).eof()) {
            displayer.getErrorStream() << "Invalid input.\n";
            return true;
        }

        if(exchange(letter, displayer, rng)) {
            nextTurn(displayer);
        }
    } else {
        displayer.getErrorStream() << "Player " << player.getId() << " couldn't make any move.\n"
                << "Turn has been skipped.\n";

        displayer.draw(board, players, player, moves_left);
        displayer.clearErrors();
        cout << "Press ENTER to continue . . . " << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        nextTurn(displayer);
    }

    return true;
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

bool Game::validateMove(Position position, ostream &error_stream) const {
    if(!position.inLimits(board.getWidth(), board.getHeight())) {
        error_stream << "Position is outside board limits.\n";
        return false;
    }

    const Board &board = as_const(this->board); // Cast board to const

    if(board.getCell(position).isEmpty()) {
        error_stream << "The given position has no letter.\n";
        return false;
    }

    if(board.getCell(position).isCovered()) {
        error_stream << "That position has already been covered.\n";
        return false;
    }

    if(!board.getCell(position).isCoverable()) {
        error_stream << "Can't move to that position.\n";
        return false;
    }

    const Player &player = players[current_player_index];
    char l = board.getCell(position).getLetter();
    if(!player.getHand().hasLetter(l)) {
        error_stream << "You don't have letter '" << l << "' in your hand.\n";
        return false;
    }

    return true;
}

void Game::_move(Position position, GameDisplayer &displayer) {
    Player &player = players[current_player_index];
    char letter = as_const(board).getCell(position).getLetter();

    moves_left -= 1;
    player.getHand().useLetter(letter);

    vector<Word> completed_words;
    board.cover(position, completed_words);

    if(completed_words.size() != 0) {
        displayer.draw(board, players, player, moves_left);
        displayer.animateWordComplete(player, completed_words);
    }
    
    player.addScore(completed_words.size());
}

bool Game::move(Position position, GameDisplayer &displayer) {
    if(!validateMove(position, displayer.getErrorStream())) return false;
    _move(position, displayer);

    return true;
}

bool Game::move(Position position, GameDisplayer &displayer, vector<Position> &legal_moves) {
    if(!validateMove(position, displayer.getErrorStream())) return false;
    
    bool is_legal = find(legal_moves.begin(), legal_moves.end(), position) != legal_moves.end();

    if(!is_legal) {
        displayer.getErrorStream() << "There is at least one move that allows you to play twice this turn. This move would only allow you to play once.\n";
        return false;
    }
    
    _move(position, displayer);

    return true;
}

bool Game::exchange(char letter, GameDisplayer &displayer, default_random_engine &rng) {
    Player &player = players[current_player_index];
    letter = toupper(letter);
    if(letter < 'A' || letter > 'Z') {
        displayer.getErrorStream() << "Character '" << letter << "' is not a letter.\n";
        return false;
    }

    if(!player.getHand().hasLetter(letter)) {
        displayer.getErrorStream() << "You don't have letter '" << letter << "' in your hand.\n";
        return false;
    }

    displayer.animateExchange(letter);
    player.getHand().exchange(pool, letter, displayer.getSwapLetterCallback());

    pool.shuffle(rng);
    return true;
}

bool Game::exchange(char letter1, char letter2, GameDisplayer &displayer, default_random_engine &rng) {
    Player &player = players[current_player_index];
    letter1 = toupper(letter1);
    if(letter1 < 'A' || letter1 > 'Z') {
        displayer.getErrorStream() << "Character '" << letter1 << "' is not a letter.\n";
        return false;
    }

    letter2 = toupper(letter2);
    if(letter2 < 'A' || letter2 > 'Z') {
        displayer.getErrorStream() << "Character '" << letter2 << "' is not a letter.\n";
        return false;
    }

    bool has_letters = true;

    if(!player.getHand().hasLetter(letter1)) {
        displayer.getErrorStream() << "You don't have letter '" << letter1 << "' in your hand.\n";
        has_letters = false;
    }

    if(!player.getHand().hasLetter(letter2)) {
        displayer.getErrorStream() << "You don't have letter '" << letter2 << "' in your hand.\n";
        has_letters = false;
    }

    if(!has_letters) return false;

    displayer.animateExchange(letter1, letter2);
    player.getHand().exchange(pool, letter1, letter2, displayer.getSwapLetterCallback());

    pool.shuffle(rng);
    return true;
}

void Game::nextTurn(GameDisplayer &displayer) {
    Player &player = players[current_player_index];
    

    if(!player.getHand().isFull()) {
        displayer.clearTurnInfo();
        displayer.draw(board, players, player, 0);

        if(pool.isEmpty()) displayer.noticeEmptyPool();
        else {
            displayer.animateRefillHand();
            player.getHand().refill(pool, displayer.getSwapLetterCallback());

            if(pool.isEmpty()) displayer.noticeDepletedPool();
            else displayer.delayAfterRefill();
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
