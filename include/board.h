#ifndef BOARD_H
#define BOARD_H

#define SIZE 10
#define EMPTY '.'
#define SHIP '#'
#define HIT 'X'
#define MISS 'O'

typedef struct {
    char field[SIZE][SIZE];
    char visible[SIZE][SIZE];
} Board;

void initBoard(Board *board);

void printBoard(char field[SIZE][SIZE], int showShips);

void clearScreen(void);

#endif
