#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side)
{
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // Set up a copy of the board
    aiBoard = new Board();

    // Set the AI's side
    aiSide = side;

    // Compute opponent's side
    if (aiSide == BLACK)
        opponentsSide = WHITE;
    else
        opponentsSide = BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player()
{
    delete aiBoard;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft)
{
    Move *move;

    // Simplest possible move - random choice
    move = doRandomMove(opponentsMove, msLeft);

    // Beat SimplePlayer - use heuristics
    // move = doHeuristicMove(opponentsMove, msLeft);

    return move;
}

/*
 * Compute AI's next move given opponent's move
 * Move is computed in simplest possible way - random one picked
 *
 */
Move *Player::doRandomMove(Move *opponentsMove, int msLeft)
{
    // Populate board with opponent's move
    aiBoard->doMove(opponentsMove, opponentsSide);

    // Calculate the simplest possible valid move - choose randomly
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j);
            if (aiBoard->checkMove(move, aiSide))
            {
                aiBoard->doMove(move, aiSide);
                return move;
            }
        }
    }
    return nullptr;
}

/*
 * Compute AI's next move given opponent's last move
 * Aim is to defeat player which plays random moves
 * Use heuristic to determine where to play
 */
Move *Player::doHeuristicMove(Move *opponentsMove, int msLeft)
{
    aiBoard->doMove(opponentsMove, opponentsSide);

    return nullptr;
}
