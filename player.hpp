#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <climits>
#include <cfloat>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *doRandomMove(Move *opponentsMove, int msLeft);
    Move *doHeuristicMove(Move *opponentsMove, int msLeft);
    Move *doMinimaxMove(Move *opponentsMove, int msLeft);
    double negamax(Move *move, int depth, Side side, double alpha, double beta);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

    Board *aiBoard;
    Side aiSide;
    Side opponentsSide;
    Move *bestMove;
    Board *test;
};

#endif
