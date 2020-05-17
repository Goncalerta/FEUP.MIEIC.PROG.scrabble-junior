#include <iostream>
#include <cctype>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include "game.h"
#include "cmd.h"

using namespace std;

Game::Game(Board &board, unsigned int num_players):
    board(board),
    pool(board.getLettersInBoard()),
    displayer(board.getWidth(), board.getHeight()),
    current_player_index(0),
    moves_left(2)
{
    for(unsigned int i = 1; i <= num_players; i++) {
        Player player(i);
        players.push_back(player);
    }
}

bool Game::isOver() const {
    return board.isFullyCovered();
}

const Player& Game::getCurrentPlayer() const {
    return players[current_player_index];
}

Player& Game::getCurrentPlayer() {
    return players[current_player_index];
}

std::vector<int> Game::getWinnersId() const {
    int winner_score = players[0].getScore();
    vector<int> winners_id;

    for(const Player &player: players) {
        int score = player.getScore();
        int id = player.getId();
        
        if(score == winner_score) winners_id.push_back(id);
        else break;
    }

    return winners_id;
}

TurnState Game::getTurnState() const {
    if(board.hasMove(getCurrentPlayer().getHand())) {
        // Current player can move.
        // Must always move if possible.
        return MUST_MOVE;
    } else if(moves_left == 1) {
        // Current player can't move, but already moved once this turn.
        // The second move is be skipped and turn is over.
        return MUST_END_TURN;
    } else if(pool.size() >= 2) {
        // Current player hasn't moved this turn and can't move.
        // Must exchange two letters with the pool.
        return MUST_EXCHANGE_TWO;
    } else if(pool.size() == 1) {
        // Current player hasn't moved this turn and can't move.
        // Pool only has one letter.
        // Must exchange one letter with the pool.
        return MUST_EXCHANGE_ONE;
    } else {
        // Current player hasn't moved this turn and can't move.
        // Pool is empty.
        // There is no choice but to skip the turn.
        return MUST_SKIP_TURN;
    }
}

bool Game::play(default_random_engine &rng) {
    pool.shuffle(rng);
    
    for(Player &player: players) {
        player.getHand().refill(pool);
    }

    clrscr();
    if(!playLoop(rng)) return false;

    // At the end players are ordered by score for the leaderboard
    stable_sort(players.begin(), players.end(), 
            [](auto p1, auto p2) { return p1.getScore() > p2.getScore(); });
    
    // Draw gameover screen
    clrscr();
    displayer.printBoard(board);
    displayer.printLeaderboard(players);
    displayer.declareWinners(getWinnersId(), (int) players.size());

    return true;
}

bool Game::playLoop(default_random_engine &rng) {
    ostream &error_messages = displayer.getErrorStream();

    while(!isOver()) {
        TurnState turn_state = getTurnState();
        GameDisplayer::CheckLegalMove is_legal = nullptr;
        bool must_play_twice = false;
        
        if(turn_state == MUST_MOVE) {
            getCheckLegalMove(must_play_twice, is_legal);
        }

        if(turn_state == MUST_EXCHANGE_TWO) {
            error_messages << "Player " << getCurrentPlayer().getId() << " couldn't make any move.\n"
                    << "Choose two letters to exchange with the Pool this turn.\n";
        } else if(turn_state == MUST_EXCHANGE_ONE) {
            error_messages << "Player " << getCurrentPlayer().getId() << " couldn't make any move.\n"
                    << "Choose a letter this turn to exchange for the remaining one in the Pool.\n";
        } else if(turn_state == MUST_END_TURN) {
            error_messages << "Player " << getCurrentPlayer().getId() << " couldn't make any more moves this turn.\n";
        } else if(turn_state == MUST_SKIP_TURN) {
            error_messages << "Player " << getCurrentPlayer().getId() << " couldn't make any move.\n"
                    << "Turn has been skipped.\n";            
        }

        gotoxy(0, 0);
        displayer.printBoard(board, is_legal);
        displayer.printScoreboard(players);
        displayer.printTurnInfo(getCurrentPlayer(), moves_left);
        displayer.clearErrors();

        setcolor(GameDisplayer::TEXT_COLOR);
        if(turn_state == MUST_END_TURN || turn_state == MUST_SKIP_TURN) {
            cout << "Press ENTER to continue . . . " << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            nextTurn();
            continue;
        }

        if(turn_state == MUST_MOVE) {
            cout << "Enter a valid position on the board to play (in the form 'Ab'): ";
        } else if(turn_state == MUST_EXCHANGE_TWO) {
            cout << "Input two letters to exchange with the Pool: ";
        } else if(turn_state == MUST_EXCHANGE_ONE) {
            cout << "Input a letter to exchange with the Pool: ";
        }

        string input;
        getline(cin, input);
        if(cin.fail()) return false;
        stringstream input_stream(input);

        if(turn_state == MUST_MOVE) {
            Position position;
            if(!parsePosition(input_stream, position)) continue;
            if(!validateMove(position, must_play_twice, is_legal)) continue;
            
            move(position);
            if(moves_left == 0) nextTurn();
            continue;
        }

        if(turn_state == MUST_EXCHANGE_TWO) {
            char letter1, letter2;
            if(!parseLetters(input_stream, letter1, letter2)) continue;
            if(!validateExchange(letter1, letter2)) continue;

            exchange(letter1, letter2, rng);
            nextTurn();
            continue;
        }

        if(turn_state == MUST_EXCHANGE_ONE) {
            char letter;
            if(!parseLetter(input_stream, letter)) continue;
            if(!validateExchange(letter)) continue;
            
            exchange(letter, rng);
            nextTurn();
            continue;
        }
    }

    return true;
}

void Game::getCheckLegalMove(bool &must_play_twice, GameDisplayer::CheckLegalMove &is_legal) const {
    vector<Position> legal_positions;
            
    if(moves_left == 2) {
        const Hand &hand = getCurrentPlayer().getHand();
    must_play_twice = board.mustPlayTwiceEdgeCase(hand, legal_positions);
    } else {
        must_play_twice = false;
    }

    if(must_play_twice) {
        is_legal = [legal_positions](Position position, auto) {
            auto begin = legal_positions.begin();
            auto end = legal_positions.end();
            return find(begin, end, position) != end;
        };
    } else {
        const Hand &hand = getCurrentPlayer().getHand();
        is_legal = [hand](auto, const Cell &cell) {
            return cell.isCoverable() && hand.hasLetter(cell.getLetter());
        };
    }
}

bool Game::parsePosition(istream &input, Position &position) {
    ostream &error_messages = displayer.getErrorStream();
    string position_str;

    input >> position_str;
        
    if(input.fail()) {
        error_messages << "Expected position in the form 'Aa'.\n";
        return false;
    }

    std::string unexpected;
    input >> unexpected;
    if(unexpected.size() != 0) {
        error_messages << "Unexpected: '" << unexpected << "'\n";
        return false;
    }

    if(position_str.size() != 2) {
        error_messages << "Couldn't parse '" << position_str
                << "' as a position.\n Use an uppercase letter followed by a lowercase one, like 'Aa'.\n";
        return false;
    }

    char x_char = position_str[1];
    char y_char = position_str[0];
    
    if(x_char < 'a' || x_char > 'z' || y_char < 'A' || y_char > 'Z') {
        error_messages << "Couldn't parse '" << position_str
                << "' as a position. Use an uppercase letter followed by a lowercase one, like 'Aa'.\n";
        return false;
    }

    position = Position(x_char, y_char);

    return true;
}

bool Game::validateMove(Position position, bool must_play_twice, GameDisplayer::CheckLegalMove is_legal) {
    ostream &error_messages = displayer.getErrorStream();

    if(!position.inLimits(board.getWidth(), board.getHeight())) {
        error_messages << "Position '" << position << "' is outside board limits.\n";
        return false;
    }

    const Cell &cell = board.getCell(position);

    if(cell.isEmpty()) {
        error_messages << "Position '" << position << "' has no letter.\n";
        return false;
    }

    if(cell.isCovered()) {
        error_messages << "Position '" << position << "' has already been covered.\n";
        return false;
    }

    if(!cell.isCoverable()) {
        error_messages << "Can't move to position '" << position << "'.\n";
        return false;
    }

    char letter = cell.getLetter();
    if(!getCurrentPlayer().getHand().hasLetter(letter)) {
        error_messages << "You don't have letter '" << letter << "' in your hand.\n";
        return false;
    }

    if(must_play_twice && !is_legal(position, board.getCell(position))) {
        error_messages << "There is at least one move that allows you to play twice this turn.\n" 
                << "Position '" << position << "' would only allow you to play once.\n";

        return false;
    }

    return true;
}

void Game::move(Position position) {
    Player &current_player = getCurrentPlayer();
    char letter = board.getCell(position).getLetter();

    moves_left -= 1;
    current_player.getHand().useLetter(letter);

    vector<Word> completed_words;
    board.cover(position, completed_words);

    if(completed_words.size() != 0) {
        // Update screen and animate word
        gotoxy(0, 0);
        displayer.printBoard(board);
        displayer.printScoreboard(players);
        displayer.animateWordComplete(current_player, completed_words);
    }
    
    current_player.addScore((unsigned int) completed_words.size());
}

bool Game::parseLetter(istream &input, char &letter) {
    ostream &error_messages = displayer.getErrorStream();

    input >> letter;
    letter = (char) toupper(letter);

    if(input.fail()) {
        error_messages << "Expected one letter.\n";
        return false;
    }

    std::string unexpected;
    input >> unexpected;
    if(unexpected.size() != 0) {
        error_messages << "Unexpected: '" << unexpected << "'\n";
        return false;
    }

    return true;
}

bool Game::validateExchange(char letter) {
    ostream &error_messages = displayer.getErrorStream();

    if(letter < 'A' || letter > 'Z') {
        error_messages << "Character '" << letter << "' is not a letter.\n";
        return false;
    }

    if(!getCurrentPlayer().getHand().hasLetter(letter)) {
        error_messages << "You don't have letter '" << letter << "' in your hand.\n";
        return false;
    }

    return true;
}

void Game::exchange(char letter, default_random_engine &rng) {
    stringstream notice;
    notice << "Exchanging letter '" << letter << "' . . .";
    displayer.notice(notice.str(), true);
    
    Hand &current_player_hand = getCurrentPlayer().getHand();
    current_player_hand.exchange(pool, letter, displayer.getSwapLetterCallback());

    pool.shuffle(rng);
}

bool Game::parseLetters(istream &input, char &letter1, char &letter2) {
    ostream &error_messages = displayer.getErrorStream();

    input >> letter1 >> letter2;
    letter1 = (char) toupper(letter1);
    letter2 = (char) toupper(letter2);

    if(input.fail()) {
        error_messages << "Expected two letters.\n";
        return false;
    }

    std::string unexpected;
    input >> unexpected;
    if(unexpected.size() != 0) {
        error_messages << "Unexpected: '" << unexpected << "'\n";
        return false;
    }

    return true;
}

bool Game::validateExchange(char letter1, char letter2) {
    ostream &error_messages = displayer.getErrorStream();

    if(letter1 < 'A' || letter1 > 'Z') {
        error_messages << "Character '" << letter1 << "' is not a letter.\n";
        return false;
    }

    if(letter2 < 'A' || letter2 > 'Z') {
        error_messages << "Character '" << letter2 << "' is not a letter.\n";
        return false;
    }

    if(letter1 == letter2) {
        // Same letter: player must have at least two 
        int hand_count = getCurrentPlayer().getHand().countLetter(letter1);
        if(hand_count == 0) {
            error_messages << "You don't have letter '" << letter1 << "' in your hand.\n";
            return false;
        } else if(hand_count == 1) {
            error_messages << "You only have one letter '" << letter1 << "' in your hand.\n";
            return false;
        }

        return true;
    } else {
        // Different letters: player must have both
        bool has_letters = true;

        if(!getCurrentPlayer().getHand().hasLetter(letter1)) {
            error_messages << "You don't have letter '" << letter1 << "' in your hand.\n";
            has_letters = false;
        }

        if(!getCurrentPlayer().getHand().hasLetter(letter2)) {
            error_messages << "You don't have letter '" << letter2 << "' in your hand.\n";
            has_letters = false;
        }

        return has_letters;
    }
}

void Game::exchange(char letter1, char letter2, default_random_engine &rng) {
    stringstream notice;
    notice << "Exchanging letters '" << letter1 << "' and '" << letter2 << "' . . .";
    displayer.notice(notice.str(), true);
    
    Hand &current_player_hand = getCurrentPlayer().getHand();
    current_player_hand.exchange(pool, letter1, letter2, displayer.getSwapLetterCallback());

    pool.shuffle(rng);
}

void Game::nextTurn() {
    Player &current_player = getCurrentPlayer();

    if(!current_player.getHand().isFull()) {
        gotoxy(0, 0);
        displayer.printBoard(board);
        displayer.printScoreboard(players);
        displayer.printTurnInfo(current_player, moves_left);

        if(pool.isEmpty()) {
            displayer.notice("The Pool is empty . . .");
        } else {
            displayer.notice("Refilling hand . . .", true);
            current_player.getHand().refill(pool, displayer.getSwapLetterCallback());

            displayer.afterRefill(pool.isEmpty());
        }
    }
    
    moves_left = 2;
    current_player_index++;
    if(current_player_index == players.size()) current_player_index = 0;
}
