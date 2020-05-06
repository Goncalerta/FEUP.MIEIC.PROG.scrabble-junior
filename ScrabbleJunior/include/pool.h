#ifndef POOL_H
#define POOL_H

#include <string>
#include <vector>
#include <random>

class Pool {
    std::vector<char> letters;

    public:
    Pool();
    Pool(std::vector<char> &letters);

    void pushLetter(char c);
    char popLetter();
    
    void shuffle(std::default_random_engine &rng);
    
    bool isEmpty() const;
    int size() const;

    void exchange(char *letter);
    void exchange(char *letter1, char *letter2);
};

#endif