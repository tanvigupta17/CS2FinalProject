// README.TXT
// AI Improvements

/*

Minimax:
- Edited iteration of depth variable in order to get minimax to work appropriately

Heuristic Evaluations:
- Previously, heuristic function simply returned static weight of given board position, i.e. single integer from 0 to 10.
- Updated static weights to include better range of relative favorabilities. Using 2D array to store static weight of each board position.
- getStaticWeight function returns static score based on current board state. Score = static weights of all AI pieces - static weights of all opponent pieces.

- Added coin parity and actual mobility heuristics to heuristic value calculation. Coin parity is difference in current score for each side, actual mobility is difference in current legal moves for each side.
- Tested multiple weights to determine how much importance to give to each heuristic, decided on 25 for coin parity and 10 for mobility.
- Final heuristic value calculated with product of heuristic score and static weight of the board.

 */
