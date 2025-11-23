#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "board.h"

// Handles a shot on the board
// Returns: 1 - hit, 0 - miss, -1 - error
int shoot(Board *board, int row, int col);

// Checks if the player has won (all ships are destroyed)
int checkWin(Board *board);

// Computer's move with simple AI
void computerMove(Board *board, int *lastHitRow, int *lastHitCol, int *direction);

// Game against the computer
void playAgainstComputer(void);

// Two-player game
void playTwoPlayers(void);

#endif
