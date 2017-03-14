// README.TXT
// AI Improvements

/*

Minimax:
- Edited iteration of depth variable in order to get minimax to work appropriately.
- Noticed that AI beats BetterPlayer with depth 4, but not depth 5, attrbuted to single round being 2-turn (need 2 turns to go back to original player's turn).

Heuristic Evaluations:
- Previously, heuristic function simply returned static weight of given board position, i.e. single integer from 0 to 10.

- Updated static weights to include better range of relative favorabilities. Using 2D array to store static weight of each board position.
- getStaticWeight function returns static score based on current board state. Score = static weights of all AI pieces - static weights of all opponent pieces.

- Added coin parity heuristic - weighted difference of current score for each side.
- Added actual mobility heuristic - weighted difference of current legal moves for each side.
- Added corner heuristic - weighted difference of corner stones for each side.
- Added frontiers heuristic - weighted difference of stones adjacent to atleast one empty space for each side.
- Note that coin parity, mobility, and corners are maximized for the AI, but aim is to minimize frontiers since they lead to losses.
- Tested multiple weights to determine how much importance to give to each heuristic; decided on 25 each for coin parity and frontiers, 35 for corners (since capturing corners greatly increases stability), and 10 for mobility.
- Final heuristic value calculated with product of heuristic score and static weight of the board.

- Tried but failed: Heuristic for closeness to corners - don't want to play pieces in spaces adjacent to corners since they give away corners. However, this heuristic didn't help the AI so is not included in final version.


References:
- Inspiration for heuristics and static weights: https://courses.cs.washington.edu/courses/cse573/04au/Project/mini1/RUSSIA/Final_Paper.pdf
- Knowledge of move stability: http://www.pressibus.org/ataxx/autre/minimax/node3.html
- Inspiration for heuristics: http://mkorman.org/othello.pdf
- Inspiration for heuristics: http://ai.stanford.edu/~chuongdo/papers/demosthenes.pdf
 */