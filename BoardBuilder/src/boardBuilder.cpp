#include "boardBuilder.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <thread>
#include "cmd.h"

using namespace std;

const char* BoardBuilder::DICTIONARY = "WORDS.txt";

const Color BoardBuilder::TEXT_COLOR = LIGHTGRAY;
const Color BoardBuilder::TEXT_COLOR_DARK = DARKGRAY;
const Color BoardBuilder::ERROR_COLOR = RED;
const Color BoardBuilder::WARNING_COLOR = YELLOW;
const Color BoardBuilder::SUCCESS_COLOR = GREEN;
const Color BoardBuilder::LETTER_COLOR = BLACK;
const Color BoardBuilder::BOARD_BACKGROUND = LIGHTGRAY;

const int BoardBuilder::ADD_LETTER_TO_BOARD_DELAY = 200;

BoardBuilder::BoardBuilder(std::string &board_name, Board &board):
    board_name(move(board_name)),
    board(board),
    board_info_x_offset(board.getWidth()*2 + 2),
    prompt_y_offset(max(8u, board.getHeight() + 2)),
    max_players(board.countLetters()/7)
{}

void BoardBuilder::printBoard() const {
    setcolor(TEXT_COLOR);
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
    Position position = word.getStart();
    Orientation orientation = word.getOrientation();
    
    int letters = board.countLetters();

    for(const char &c: word) {
        if(board.getCell(position).isEmpty()) {
            int x = position.getX()*2 + 1;
            int y = position.getY() + 1;

            gotoxy(x, y);
            setcolor(LETTER_COLOR, BOARD_BACKGROUND);
            cout << c;

            letters += 1;
            gotoxy(board_info_x_offset+19, 4);
            setcolor(TEXT_COLOR);
            cout << letters;

            this_thread::sleep_for(chrono::milliseconds(ADD_LETTER_TO_BOARD_DELAY));
        }

        position.stepForward(orientation);
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
    switch(max_players) {
        case 0:
        case 1:
            // 0 and 1 are below minimum number (2)
            setcolor(ERROR_COLOR);
            cout << "Not playable yet";
            break;
        case 2:
            setcolor(TEXT_COLOR_DARK);
            cout << "Playable by "; 
            setcolor(TEXT_COLOR);
            cout << "2 players";
            break;
        default:
            setcolor(TEXT_COLOR_DARK);
            cout << "Playable by "; 
            setcolor(TEXT_COLOR);
            cout << "2-" << max_players << " players";
            break;
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
    max_players = min(4u, board.countLetters()/7);

    printBoardInfo();
    clrscr(0, prompt_y_offset);

    switch(max_players) {
        case 0:
        case 1:
            setcolor(ERROR_COLOR);
            cout << "This board needs " << 14-board.countLetters() << " more letters in order to be playable by at least 2 players." << endl << endl;
            break;
        case 2:
        case 3:
            setcolor(WARNING_COLOR);
            cout << "This board needs " << (max_players+1)*7-board.countLetters() << " more letters in order to be playable by at least " << max_players+1 << " players." << endl << "Ctrl+Z to save this board to a file and quit." << endl << endl;
            break;
        case 4:
            setcolor(SUCCESS_COLOR);
            cout << "This board is playable by 2-4 players." << endl << "Ctrl+Z to save this board to a file and quit." << endl << endl;
            break;
    }

    setcolor(TEXT_COLOR);
    cout << "Inputs must be in the form 'Aa H WORD': " << endl
            << "  - The starting position of the word;" << endl
            << "  - The orientation ('H' for horizontal, 'V' for vertical);" << endl
            << "  - A valid word to place on the board." << endl << endl;

    setcolor(ERROR_COLOR);
    cout << error_messages.str();
    setcolor(TEXT_COLOR);
    error_messages.str("");

    cout << "Enter valid position in the form described above: ";

    string input_line;
    getline(cin, input_line);
    if(cin.fail()) return false;

    stringstream input_line_stream(input_line);
    
    char cpos[2];
    char corientation;
    string cword;

    input_line_stream >> cpos;
    char cx = cpos[1];
    char cy = cpos[0];

    // TODO dont allow things like 'A a h test'
    if(input_line_stream.fail()) {
        error_messages << "Expected position in the form 'Aa'.\n";
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

    auto invalid_char = find_if_not(cword.begin(), cword.end(), isalpha);
    if(invalid_char != cword.end()) {
        setcolor(ERROR_COLOR);
        
        error_messages << "Only allowed words with ASCII alphabetic letters.\n";
        if(isspace(*invalid_char)) error_messages << "Whitespace is not allowed." << endl;
        else error_messages << "'" << *invalid_char << "' is not allowed." << endl;

        return true;
    }

    if(cword.size() < 2) {
        error_messages << "Only allowed words with at least two letters.\n";
        return true;
    }

    std::string unexpected_argument;
    input_line_stream >> unexpected_argument;
    if(unexpected_argument.size() != 0) {
        error_messages << "Unexpected: '" << unexpected_argument << "'\n";
        return true;
    }

    transform(cword.begin(), cword.end(), cword.begin(), toupper);

    ifstream dict(DICTIONARY);
    if(!dict.is_open()) {
        error_messages << "Dictionary file '" << DICTIONARY << "' was not found in the current folder or could not be oppened.\n"
            << "Make sure such file exists and can be opened before inputing your word again.\n";
        return true;
    }
    
    if(!inDict(dict, cword)) {
        error_messages << "Word '" << cword << "' not found in dictionary\n";
        return true;
    }

    Word word(Position(cx, cy), parseOrientation(corientation), cword);

    if(!board.isWordValid(word, error_messages)) return true;

    printNewWord(word);
    board.addWord(word);
    
    return true;
}

void BoardBuilder::saveToFile() const {
    if(max_players < 2) {
        setcolor(WARNING_COLOR);
        cout << "Board does not have enough letters in order to be playable." << endl
                << "Board will still be saved and may be edited later." << endl;
    }
    
    string filename = board_name + ".txt";
    ofstream outfile(filename);

    while(!outfile.is_open()) {
        if(cin.fail()) return;
        
        setcolor(ERROR_COLOR);
        cout << "Could not open '" << filename << "' for writing." << endl;
        setcolor(TEXT_COLOR);
        cout << "Make sure such file can be opened for writting, then press ENTER: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        outfile.open(filename);
    }

    board.writeData(outfile);
    setcolor(SUCCESS_COLOR);
    cout << "Board has been successfully saved to '" << filename << "'." << endl;
    setcolor(WHITE);
    cout << "Press ENTER to exit: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool BoardBuilder::inDict(istream &dict, string word) {
    string dict_word;

    while(dict >> dict_word) {
        transform(dict_word.begin(), dict_word.end(), dict_word.begin(), toupper);
        if(word == dict_word) return true;
    } 
    
    return false;
}
