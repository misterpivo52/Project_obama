#include <stdio.h>
#include "../include/board.h"

void initBoard(Board *board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board->field[i][j] = EMPTY;
            board->visible[i][j] = EMPTY;
        }
    }
}

void printBoard(char field[SIZE][SIZE], int showShips) {
    printf("\n  ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            if (!showShips && field[i][j] == SHIP) {
                printf("%c ", EMPTY);
            } else {
                printf("%c ", field[i][j]);
            }
        }
        printf("\n");
    }
}

void clearScreen(void) {
    for (int i = 0; i < 20; i++) {
        printf("\n");
    }
    printf("=========================================\n");
}
