#include "boardBuilder.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include "cmd.h"

using namespace std;

const char* DICTIONARY = "WORDS.txt";

const Color BoardBuilder::TEXT_COLOR = LIGHTGRAY;
const Color BoardBuilder::TEXT_COLOR_DARK = DARKGRAY;
const Color BoardBuilder::ERROR_COLOR = RED;
const Color BoardBuilder::WARNING_COLOR = YELLOW;
const Color BoardBuilder::OK_COLOR = GREEN;
const Color BoardBuilder::LETTER_COLOR = BLACK;
const Color BoardBuilder::BOARD_BACKGROUND = LIGHTGRAY;

const int BoardBuilder::ADD_LETTER_TO_BOARD_DELAY = 100;

BoardBuilder::BoardBuilder(std::string &board_name, Board &board):
    board_name(move(board_name)),
    board(board),
    board_info_x_offset(board.getWidth()*2 + 2),
    prompt_y_offset(max(8u, board.getHeight() + 2)),
    max_players(0)
{}

void BoardBuilder::printBoard() const {
    cout << ' ';
    for(char letter = 'a'; letter < board.getWidth() + 'a'; letter++) {
        cout << letter << ' ';
    }
    cout << '\n';

    for(int j = 0; j < board.getHeight(); j++) {
        char letter = j + 'A';
        cout << letter;
        
        setcolor(LETTER_COLOR, BOARD_BACKGROUND);
        for(int i = 0; i < board.getWidth(); i++) {
            cout << board.getCell(Position(i, j));
            
            if(i+1 != board.getWidth()) {
                cout << ' ';
            }
        }
        setcolor(TEXT_COLOR);
        cout << '\n';
    }
}

void BoardBuilder::printNewWord(const Word &word) const {
    int x = word.getStart().getX()*2 + 1;
    int y = word.getStart().getY() + 1;

    int letters = board.countLetters();

    for(const char &c: word) {
        gotoxy(x, y);
        setcolor(TEXT_COLOR);
        cout << c;


        gotoxy(board_info_x_offset+19, 4);
        setcolor(LETTER_COLOR, BOARD_BACKGROUND);
        cout << letters;

        this_thread::sleep_for(chrono::milliseconds(ADD_LETTER_TO_BOARD_DELAY));

        if(word.getOrientation() == Horizontal) x += 2;
        else y += 1;

        letters += 1;
    }

    gotoxy(board_info_x_offset+17, 5);
    setcolor(TEXT_COLOR);
    cout << board.countWords() + 1;
}

void BoardBuilder::printBoardInfo() const {
    gotoxy(board_info_x_offset, 1);
    setcolor(TEXT_COLOR_DARK);
    cout << "Board name: "; 
    setcolor(TEXT_COLOR);
    cout << board_name;

    gotoxy(board_info_x_offset, 2);
    if(board.countLetters() >= 21) {
        setcolor(TEXT_COLOR_DARK);
        cout << "Playable by "; 
        setcolor(TEXT_COLOR);
        cout << '2-' << board.countLetters()/7 << " players";
    } else if(board.countLetters() >= 14) {
        setcolor(TEXT_COLOR_DARK);
        cout << "Playable by "; 
        setcolor(TEXT_COLOR);
        cout << "2 players";
    } else {
        setcolor(ERROR_COLOR);
        cout << "Not playable yet";
    }

    gotoxy(board_info_x_offset, 4);
    setcolor(TEXT_COLOR_DARK);
    cout << "Number of letters: ";
    setcolor(TEXT_COLOR);
    cout << board.countLetters(); 

    gotoxy(board_info_x_offset, 5);
    setcolor(TEXT_COLOR_DARK);
    cout << "Number of words: ";
    setcolor(TEXT_COLOR);
    cout << board.countWords(); 
}

bool BoardBuilder::loop() {
    printBoardInfo();
    clrscr(0, prompt_y_offset);
    
    cout << "Inputs must be in the form 'Aa H WORD': " << endl
            << "  - The starting position of the word;" << endl
            << "  - The orientation ('H' for horizontal, 'V' for vertical);" << endl
            << "  - A valid word to place on the board." << endl << endl;

    setcolor(ERROR_COLOR);
    cout << error_messages.str();
    setcolor(TEXT_COLOR);
    error_messages.str("");

    cout << "Enter valid position in the form above: ";

    string input_line;
    getline(cin, input_line);
    if(cin.fail()) return false;

    stringstream input_line_stream(input_line);
    
    char cx, cy, corientation;
    string cword;

    input_line_stream >> cy >> cx;
    if(input_line_stream.fail()) {
        error_messages << "Expected position in the form Ab.\n";
        return true;
    }
    
    if(cx < 'a' || cx > 'z' || cy < 'A' || cy > 'Z') {
        error_messages << "Couldn't parse '" << cy << cx << "' as a position. Use an uppercase letter followed by a lowercase one.\n" << endl;
        return true;
    }

    input_line_stream >> corientation;
    
    if(input_line_stream.fail()) {
        error_messages << "Expected orientation ('H' or 'V').\n";
        return true;
    }

    if(corientation != 'H' && corientation != 'h' && corientation != 'V' && corientation != 'v') {
        error_messages << "Couldn't parse '" << corientation << "' as orientation ('H' or 'V').\n";
        return true;
    }

    input_line_stream >> cword;

    if(input_line_stream.fail()) {
        error_messages << "Expected a word.\n";
        return true;
    }

    if(cword.size() < 2) {
        error_messages << "Only allowed words with at least two letters.\n";
        return true;
    }

    auto invalid_char = find_if_not(cword.begin(), cword.end(), isalpha);
    if(invalid_char != cword.end()) {
        setcolor(ERROR_COLOR);
        
        error_messages << "Only allowed words with ASCII alphabetic letters.\n";
        if(isspace(*invalid_char)) error_messages << "Whitespace is not allowed." << endl;
        else error_messages << "'" << *invalid_char << "' is not allowed." << endl;

        return true;
    }

    std::string unexpected_argument;
    input_line_stream >> unexpected_argument;
    if(!input_line_stream.eof()) {
        error_messages << "Unexpected: '" << unexpected_argument << "'\n";
        return true;
    }

    //if(not in dict) {
        // error_messages << "Word '" << cword << "' not found in dictionary";
    //}

    transform(cword.begin(), cword.end(), cword.begin(), toupper);

    Word word(Position(cx, cy), parseOrientation(corientation), cword);

    Position position = word.getStart();
    Orientation orientation = word.getOrientation();

    position.stepBackwards(orientation);
    if(!position.inLimits(board.getWidth(), board.getHeight()) && !board.getCell(position).isEmpty()) {
        error_messages << "Input would be adjacent to existing word at '" << position << "', creating ambiguity.\n";
        return true;
    }

    for(char current_letter: word) {
        position.stepForward(orientation);
        if(!position.inLimits(board.getWidth(), board.getHeight())) {
            error_messages << "Input goes outside the board at position '" << position << "'.\n";
            return true;
        }
        Cell &current_cell = board.getCell(position);

        if(current_cell.isEmpty()) {
            std::pair<Position, Position> laterals = position.laterals(orientation);

            if(!position.inLimits(board.getWidth(), board.getHeight()) && !board.getCell(position).isEmpty()) {
                error_messages << "Input would be adjacent to existing word at '" << position << "', creating ambiguity.\n";
                return true;
            }
            if(!position.inLimits(board.getWidth(), board.getHeight()) && !board.getCell(position).isEmpty()) {
                error_messages << "Input would be adjacent to existing word at '" << position << "', creating ambiguity.\n";
                return true;
            }
        } else if(current_cell.getLetter() != current_letter) {
            error_messages << "Input colides with letter '" << current_cell.getLetter() << "' at position '" << position << "'.\n";
            return true;
        }
    }
    position.stepForward(orientation);
    if(!position.inLimits(board.getWidth(), board.getHeight()) && !board.getCell(position).isEmpty()) {
        error_messages << "Input would be adjacent to existing word at '" << position << "', creating ambiguity.\n";
        return true;
    }

    board.addWord(word);
    printNewWord(word);

    return true;
}

void BoardBuilder::saveToFile() const {
    // TODO what if file already exists?
    ofstream outfile(board_name + ".txt");
    board.writeData(outfile);
}
