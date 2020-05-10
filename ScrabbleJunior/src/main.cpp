#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include "board.h"
#include "pool.h"
#include "orientation.h"
#include "position.h"
#include "player.h"
#include "game.h"
#include "gameDisplayer.h"
#include "cmd.h"
#include "gameLoader.h"
#include <algorithm>
#include <limits>

using namespace std;

bool promptPlayAgain() {
    string input;
    while(true) {
        cout << endl << "Play again? (Y/N): ";
        getline(cin, input);
        if(cin.fail()) return false;

        char answer;
        stringstream input_stream(input);
        input_stream >> answer;

        if(input_stream.fail()) {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << "Invalid input." << endl;
            setcolor(GameDisplayer::TEXT_COLOR);
        } else if(answer == 'Y' || answer == 'y') return true;
        else if(answer == 'N' || answer == 'n') return false;
        else {
            setcolor(GameDisplayer::ERROR_COLOR);
            cout << "Invalid input." << endl;
            setcolor(GameDisplayer::TEXT_COLOR);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    
    do {
        GameLoader loader;
        Board board;
        int num_players;

        bool ready_to_play = false;

        while(!ready_to_play) {
            switch(loader.getState()) {
                case Unloaded:
                    loader.loadBoard(board);
                    break;

                case BoardLoaded:
                    loader.promptNumberPlayers(num_players);
                    break;

                case ReadyToPlay:
                    ready_to_play = true;
                    break;

                case FailedToLoad:
                    return 0;
            }
        }

        Game game(board, num_players);
        bool game_ended_without_error = game.play(rng);
        if(!game_ended_without_error) return 0;
    } while(promptPlayAgain());

    return 0;
}
