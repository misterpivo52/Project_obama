#include <stdio.h>
#include <stdlib.h>
#include "../include/placement.h"
#include "../include/board.h"

int canPlaceShip(Board *board, int row, int col, int size, int horizontal) {
    if (horizontal) {
        if (col + size > SIZE) return 0;
        
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j < col + size + 1; j++) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                    if (board->field[i][j] == SHIP) return 0;
                }
            }
        }
    } else {
        if (row + size > SIZE) return 0;
        
        for (int i = row - 1; i < row + size + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                    if (board->field[i][j] == SHIP) return 0;
                }
            }
        }
    }
    return 1;
}

void placeShip(Board *board, int row, int col, int size, int horizontal) {
    if (horizontal) {
        for (int j = col; j < col + size; j++) {
            board->field[row][j] = SHIP;
        }
    } else {
        for (int i = row; i < row + size; i++) {
            board->field[i][col] = SHIP;
        }
    }
}

void autoPlaceShips(Board *board) {
    int ships[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    int numShips = 10;

    for (int s = 0; s < numShips; s++) {
        int size = ships[s];
        int placed = 0;

        while (!placed) {
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            int horizontal = rand() % 2;

            if (canPlaceShip(board, row, col, size, horizontal)) {
                placeShip(board, row, col, size, horizontal);
                placed = 1;
            }
        }
    }
}

void manualPlaceShips(Board *board, char *playerName) {
    int ships[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    char *names[] = {
        "Battleship (4)",
        "Cruiser #1 (3)",
        "Cruiser #2 (3)",
        "Destroyer #1 (2)",
        "Destroyer #2 (2)",
        "Destroyer #3 (2)",
        "Submarine #1 (1)",
        "Submarine #2 (1)",
        "Submarine #3 (1)",
        "Submarine #4 (1)"
    };

    printf("\n%s, place your ships!\n", playerName);

    for (int s = 0; s < 10; s++) {
        int size = ships[s];
        int placed = 0;

        while (!placed) {
            printf("\n");
            printBoard(board->field, 1);
            printf("\nPlacing: %s\n", names[s]);
            printf("Enter row, column, and direction (0-horizontal, 1-vertical): ");

            int row, col, dir;
            if (scanf("%d %d %d", &row, &col, &dir) != 3) {
                printf("Input error!\n");
                while (getchar() != '\n');
                continue;
            }

            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
                printf("Coordinates are out of bounds!\n");
                continue;
            }

            if (canPlaceShip(board, row, col, size, dir == 0)) {
                placeShip(board, row, col, size, dir == 0);
                placed = 1;
            } else {
                printf("Cannot place a ship here!\n");
            }
        }
    }

    printf("\nShips placed! Press Enter to continue...");
    while (getchar() != '\n');
    getchar();
    clearScreen();
}
