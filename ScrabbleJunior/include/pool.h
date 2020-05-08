#ifndef POOL_H
#define POOL_H

#include <vector>
#include <random>

class Pool {
    std::vector<char> letters;

    public:
    Pool(std::vector<char> &letters);

    char popLetter();
    void exchange(char *letter);
    void exchange(char *letter1, char *letter2);

    void shuffle(std::default_random_engine &rng);

    bool isEmpty() const;
    int size() const;
};

#endif