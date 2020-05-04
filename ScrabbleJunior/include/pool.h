#ifndef POOL_H
#define POOL_H

#include <string>
#include <vector>
#include <random>

class Pool {
    std::vector<char> letters;

    public:
    Pool();
    
    void pushLetter(char c);
    void pushWord(std::string s);
    char popLetter();
    
    void shuffle(std::default_random_engine &rng);
    
    bool isEmpty() const;
    int size() const;

    void exchange(char *letter);
    void exchange(char *letter1, char *letter2);
};

#endif