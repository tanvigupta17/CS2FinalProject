#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Current count of corner stones for given side
 */
int Board::fourCorners(Side side)
{
    int count = 0;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (get(side, (i*7), (j*7)))
                count += 1;
        }
    }
    return count;
}

/*
 * Count of potential corner stones for given side
 */

/*
 * Current count of stones adjacent to corner for given side
 */
int Board::cornerCloseness(Side side)
{
    int count = 0;
    for (int i = 0; i < 8; i+=7)
    {
        for (int j = 1; j < 7; j+=5)
        {
            if (get(side, i, j))
                count += 1;
        }
    }

    for (int i = 1; i < 7; i+=5)
    {
        for (int j = 0; j < 8; j+=7)
        {
            if (get (side, i, j))
                count += 1;
        }
    }
    return count;
}

/*
 * Count of "frontier discs" for given side
 */
int Board::frontierDiscs(Side side)
{
    int count = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (get(side, i, j))
            {
                for (int k = -1; k < 2; k++)
                {
                    for (int l = -1; l < 2; l++)
                    {
                        if (onBoard((i+k), (j+l)) && 
                            !occupied((i+k), (j+l)))
                        {
                            count += 1;
                            goto loop;
                        }
                    }
                }
            }
            loop: ;
        }
    }
    return count;
}

/*
 * Get vector of possible moves for given stone.
 */
std::vector<Move *> Board::possibleMoves(Side side)
{
    std::vector<Move *> moves;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j);
            if (checkMove(move, side))
            {
                moves.push_back(move);
            }
        }
    }
    return moves;
}

/*
 * Get static weight of given board position as basic test of favorability.
 */
double Board::getStaticWeight(Side side)
{
    int aiScore = 0;
    int oppScore = 0;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (get(side, i, j))
                aiScore += staticWeights[i][j];
            else if (occupied(i, j))
                oppScore += staticWeights[i][j];
        }
    }
    return (double) (aiScore - oppScore);
}

/*
 * Get heuristic for current board state
 */
double Board::getHeuristicValue(Side side)
{
    Side other = (side == BLACK) ? WHITE : BLACK;

    // Calculate coin parity
    double aiCoins = (double) count(side);
    double oppCoins = (double) count(other);
    double coinVal = 100 * (aiCoins - oppCoins) / (aiCoins + oppCoins);

    // Calculate actual mobility
    double aiMoves = (double) possibleMoves(side).size();
    double oppMoves = (double) possibleMoves(other).size();
    double mobVal = 0;
    if ((aiMoves + oppMoves) != 0)
        mobVal = 100 * (aiMoves - oppMoves) / (aiMoves + oppMoves);

    // Calculate 4-corners
    double aiCorner = (double) fourCorners(side);
    double oppCorner = (double) fourCorners(other);
    double cornerVal = 0;
    if ((aiCorner + oppCorner) != 0)
        cornerVal = 100 * (aiCorner - oppCorner) / (aiCorner + oppCorner);

    // Calculate corner closeness - not being used currently
    /*
    double aiClose = (double) cornerCloseness(side);
    double oppClose = (double) cornerCloseness(other);
    double closeVal = 0;
    if ((aiClose + oppClose) != 0)
        closeVal = 100 * (- aiClose + oppClose) / (aiClose + oppClose);
    */

    // Calculate frontier discs
    double aiFront = (double) frontierDiscs(side);
    double oppFront = (double) frontierDiscs(other);
    double frontVal = 0;
    if ((aiFront + oppFront) != 0)
        frontVal = 100 * (- aiFront + oppFront) / (aiFront + oppFront);

    // Calculate final heuristic
    double score = getStaticWeight(side) * ((frontVal * 25) + (cornerVal * 35) + (coinVal * 25) + (mobVal * 10));

    return score;
}

int Board::getNaiveHeuristic(Move *move, Side side)
{
    Side other;
    Board *testBoard = copy();

    if (side == BLACK)
        other = WHITE;
    else
        other = BLACK;

    testBoard->doMove(move, side);
    
    int value = count(side) - count(other);

    return value;
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
