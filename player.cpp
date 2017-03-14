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

    // Globals for minimax implementation
    bestMove = nullptr;
    test = aiBoard->copy();
}

/*
 * Destructor for the player.
 */
Player::~Player()
{
    delete aiBoard;
    delete test;
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
    // move = doRandomMove(opponentsMove, msLeft);

    // Beat SimplePlayer - use heuristics
    // move = doHeuristicMove(opponentsMove, msLeft);

    // Further improve AI - use minimax
    move = doMinimaxMove(opponentsMove, msLeft);

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
    // Populate board with opponent's move
    aiBoard->doMove(opponentsMove, opponentsSide);

    Board *testBoard = nullptr;

    int x = -1;
    int y = -1;
    double tempValue;
    double maxValue = -DBL_MAX;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j);
            if (aiBoard->checkMove(move, aiSide))
            {
                testBoard = aiBoard->copy();
                testBoard->doMove(move, aiSide);
                tempValue = testBoard->getHeuristicValue(aiSide);
                if (tempValue > maxValue)
                {
                    x = move->getX();
                    y = move->getY();
                    maxValue = tempValue;
                }
                delete testBoard;
            }
        }
    }

    if (x == (-1) && y == (-1))
        return nullptr;

    Move *best = new Move(x, y);
    aiBoard->doMove(best, aiSide);

    return best;
}

/*
 * Compute best move given opponents move
 * Use 2-ply minimax to predict game state two sets of moves ahead
 * Pick move which leads to best final game state
 */
Move *Player::doMinimaxMove(Move *opponentsMove, int msLeft)
{
    double score;

    // Update disposable board to reflect current board state
    test = aiBoard->copy();

    // Reset bestMove
    bestMove = nullptr;

    // Populate board with opponent's move
    aiBoard->doMove(opponentsMove, opponentsSide);

    if (testingMinimax)
        score = negamax(opponentsMove, 2, opponentsSide, -DBL_MAX, DBL_MAX);
    else
        score = negamax(opponentsMove, 4, opponentsSide, -DBL_MAX, DBL_MAX);

    if (bestMove != nullptr)
        aiBoard->doMove(bestMove, aiSide);

    return bestMove;
}

double Player::negamax(Move *move, int depth, Side side, double alpha, double beta)
{
    // Make current move to test cases
    test->doMove(move, side);

    Board *originalTest = test->copy();

    // Base case for recursion - no possible moves or reached depth needed
    if (test->isDone() || depth <= 0)
    {
        // Determine which heuristic to use
        if (testingMinimax)
            return test->getNaiveHeuristic(move, side);
        else
            return test->getHeuristicValue(side);
    }

    std::vector<double> scores;

    double best = -DBL_MAX;

    // Switch side
    if (side == BLACK)
        side = WHITE;
    else
        side = BLACK;

    // Get possible moves for other player (side has been switched)
    std::vector<Move *> possibles = test->possibleMoves(side);

    depth -= 1;

    // Find best move in possibles
    for (unsigned int i = 0; i < possibles.size(); i++)
    {
        double score = -negamax(possibles[i], depth, side, -beta, -alpha);
        scores.push_back(score);
        best = max(best, score);
        alpha = max(alpha, score);

        delete test;
        test = originalTest->copy();

        if (alpha >= beta)
            break;
    }

    delete originalTest;

    Move *oldBest = bestMove;

    for (unsigned int j = 0; j < scores.size(); j++)
    {
        if (scores[j] == best)
        {
            bestMove = possibles[j];
            break;
        }
    }

    if (bestMove == oldBest)
        bestMove = nullptr;

    return best;
}
