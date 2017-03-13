#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <bitset>
#include <vector>
#include <cmath>
#include "common.hpp"
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    int staticWeights[8][8] = {{4, -3, 2, 2, 2, 2, -3, 4},
                               {-3, -4, -1, -1, -1, -1, -4, -3},
                               {2, -1, 1, 0, 0, 1, -1, 2},
                               {2, -1, 0, 1, 1, 0, -1, 2},
                               {2, -1, 0, 1, 1, 0, -1, 2},
                               {2, -1, 1, 0, 0, 1, -1, 2},
                               {-3, -4, -1, -1, -1, -1, -4, -3},
                               {4, -3, 2, 2, 2, 2, -3, 4}};

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    std::vector<Move *> possibleMoves(Side side);
    double getStaticWeight(Side side);
    double getHeuristicValue(Side side);
    int getNaiveHeuristic(Move *m, Side side);

    void setBoard(char data[]);
};

#endif
