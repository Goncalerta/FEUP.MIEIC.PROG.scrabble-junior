#include <iostream>

int main() {
    srand(time(0));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    // TODO ask number of players
    // TODO ask name of the board file 
    string board_file_name = "TEST.txt";
    ifstream board_file(board_file_name);

    Board board;
    Pool pool;

    if(!parseBoardFile(board_file, board, pool)) {
        cout << "Invalid board file";
        return 1; // TODO 
    }

    pool.shuffle(rng);

    Player p1;
    Player p2;
    int turn = 0;

    p1.refillHand(pool);
    p2.refillHand(pool);

    board.printGrid(cout);
    cout << "P1 hand:" << endl;
    for(auto i = p1.handBegin(); i <= p1.handEnd(); i++) {
        cout << *i << " ";
    }    
    cout << endl;
    cout << "P2 hand:" << endl;
    for(auto i = p2.handBegin(); i <= p2.handEnd(); i++) {
        cout << *i << " ";
    }    
    cout << endl;

    string p_input;
    while(getline(cin, p_input)) {
        if(p_input.size() != 2) {
            cout << "Too many chars in input";
            continue;
        }

        Position pos(p_input[1], p_input[0]);
        if(turn % 2 == 0) {
            char l = board.getLetter(pos);
            if(!p1.hasLetter(l)) {
                cout << "Doesn't have letter.";
                continue;
            }
            int score_gain = board.cover(pos);
            if(score_gain == Board::ILLEGAL_MOVE) {
                cout << "Illegal move.";
                continue;
            }
            p1.useLetter(l);
            p1.addScore(score_gain);
        } else {

        }
        turn++;
    }

    return 0;
}