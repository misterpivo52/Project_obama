#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "board.h"

int canPlaceShip(Board *board, int row, int col, int size, int horizontal);

void placeShip(Board *board, int row, int col, int size, int horizontal);

void autoPlaceShips(Board *board);

void manualPlaceShips(Board *board, char *playerName);

#endif
