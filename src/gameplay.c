#include <stdio.h>
#include <stdlib.h>
#include "../include/gameplay.h"
#include "../include/board.h"
#include "../include/placement.h"

int shoot(Board *board, int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return -1;
    }

    if (board->visible[row][col] == HIT || board->visible[row][col] == MISS) {
        return -1;
    }

    if (board->field[row][col] == SHIP) {
        board->field[row][col] = HIT;
        board->visible[row][col] = HIT;
        return 1;
    } else {
        board->visible[row][col] = MISS;
        return 0;
    }
}

int checkWin(Board *board) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board->field[i][j] == SHIP) {
                return 0;
            }
        }
    }
    return 1;
}

void computerMove(Board *board, int *lastHitRow, int *lastHitCol, int *direction) {
    int row, col;

    if (*lastHitRow != -1) {
        int found = 0;
        int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (int d = 0; d < 4 && !found; d++) {
            row = *lastHitRow + dirs[d][0];
            col = *lastHitCol + dirs[d][1];

            if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
                if (board->visible[row][col] != HIT && board->visible[row][col] != MISS) {
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            *lastHitRow = -1;
            *lastHitCol = -1;
        }
    }

    if (*lastHitRow == -1) {
        do {
            row = rand() % SIZE;
            col = rand() % SIZE;
        } while (board->visible[row][col] == HIT || board->visible[row][col] == MISS);
    }

    int result = shoot(board, row, col);

    if (result == 1) {
        printf("Computer HIT at (%d, %d)!\n", row, col);
        *lastHitRow = row;
        *lastHitCol = col;
    } else {
        printf("Computer MISSED at (%d, %d)\n", row, col);
        *lastHitRow = -1;
        *lastHitCol = -1;
    }
}

void playAgainstComputer(void) {
    Board playerBoard, computerBoard;
    initBoard(&playerBoard);
    initBoard(&computerBoard);

    printf("\nShip placement:\n");
    printf("1 - Automatic\n");
    printf("2 - Manual\n");
    printf("Choice: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 2) {
        manualPlaceShips(&playerBoard, "Player");
    } else {
        autoPlaceShips(&playerBoard);
        printf("\nShips placed automatically!\n");
    }

    autoPlaceShips(&computerBoard);

    printf("\nLet's start the game!\n");

    int lastHitRow = -1, lastHitCol = -1, direction = 0;
    int playerTurn = 1;

    while (1) {
        if (playerTurn) {
            printf("\n=== YOUR TURN ===\n");
            printf("\nYour board:\n");
            printBoard(playerBoard.visible, 1);
            printf("\nComputer's board:\n");
            printBoard(computerBoard.visible, 0);

            printf("\nEnter shot coordinates (row column): ");
            int row, col;

            if (scanf("%d %d", &row, &col) != 2) {
                printf("Input error!\n");
                while (getchar() != '\n');
                continue;
            }

            int result = shoot(&computerBoard, row, col);

            if (result == -1) {
                printf("Invalid coordinates or already shot there!\n");
                continue;
            } else if (result == 1) {
                printf("HIT!\n");
                if (checkWin(&computerBoard)) {
                    printf("\n*** VICTORY! You have sunk all enemy ships! ***\n");
                    break;
                }
            } else {
                printf("Miss!\n");
                playerTurn = 0;
            }
        } else {
            printf("\n=== COMPUTER'S TURN ===\n");
            computerMove(&playerBoard, &lastHitRow, &lastHitCol, &direction);

            if (checkWin(&playerBoard)) {
                printf("\n*** DEFEAT! The computer has sunk all your ships! ***\n");
                break;
            }

            if (playerBoard.visible[lastHitRow != -1 ? lastHitRow : 0][lastHitCol != -1 ? lastHitCol : 0] != HIT) {
                playerTurn = 1;
            }
        }
    }
}

void playTwoPlayers(void) {
    Board player1Board, player2Board;
    initBoard(&player1Board);
    initBoard(&player2Board);

    printf("\n=== PLAYER 1 SETUP ===\n");
    printf("Ship placement:\n");
    printf("1 - Automatic\n");
    printf("2 - Manual\n");
    printf("Choice: ");

    int choice1;
    scanf("%d", &choice1);

    if (choice1 == 2) {
        manualPlaceShips(&player1Board, "Player 1");
    } else {
        autoPlaceShips(&player1Board);
        printf("\nPlayer 1's ships placed automatically!\n");
        printf("Press Enter to continue...");
        while (getchar() != '\n');
        getchar();
        clearScreen();
    }

    printf("\n=== PLAYER 2 SETUP ===\n");
    printf("Ship placement:\n");
    printf("1 - Automatic\n");
    printf("2 - Manual\n");
    printf("Choice: ");

    int choice2;
    scanf("%d", &choice2);

    if (choice2 == 2) {
        manualPlaceShips(&player2Board, "Player 2");
    } else {
        autoPlaceShips(&player2Board);
        printf("\nPlayer 2's ships placed automatically!\n");
        printf("Press Enter to continue...");
        while (getchar() != '\n');
        getchar();
        clearScreen();
    }

    printf("\nLet's start the game!\n");

    int currentPlayer = 1;

    while (1) {
        if (currentPlayer == 1) {
            clearScreen();
            printf("\n=== PLAYER 1'S TURN ===\n");
            printf("\nYour board:\n");
            printBoard(player1Board.visible, 1);
            printf("\nOpponent's board:\n");
            printBoard(player2Board.visible, 0);

            printf("\nEnter shot coordinates (row column): ");
            int row, col;

            if (scanf("%d %d", &row, &col) != 2) {
                printf("Input error!\n");
                while (getchar() != '\n');
                continue;
            }

            int result = shoot(&player2Board, row, col);

            if (result == -1) {
                printf("Invalid coordinates or already shot there!\n");
                continue;
            } else if (result == 1) {
                printf("HIT!\n");
                if (checkWin(&player2Board)) {
                    printf("\n*** PLAYER 1 WINS! ***\n");
                    break;
                }
                printf("Shoot again! Press Enter...");
                while (getchar() != '\n');
                getchar();
            } else {
                printf("Miss!\n");
                printf("Press Enter to pass the turn to Player 2...");
                while (getchar() != '\n');
                getchar();
                currentPlayer = 2;
            }
        } else {
            clearScreen();
            printf("\n=== PLAYER 2'S TURN ===\n");
            printf("\nYour board:\n");
            printBoard(player2Board.visible, 1);
            printf("\nOpponent's board:\n");
            printBoard(player1Board.visible, 0);

            printf("\nEnter shot coordinates (row column): ");
            int row, col;

            if (scanf("%d %d", &row, &col) != 2) {
                printf("Input error!\n");
                while (getchar() != '\n');
                continue;
            }

            int result = shoot(&player1Board, row, col);

            if (result == -1) {
                printf("Invalid coordinates or already shot there!\n");
                continue;
            } else if (result == 1) {
                printf("HIT!\n");
                if (checkWin(&player1Board)) {
                    printf("\n*** PLAYER 2 WINS! ***\n");
                    break;
                }
                printf("Shoot again! Press Enter...");
                while (getchar() != '\n');
                getchar();
            } else {
                printf("Miss!\n");
                printf("Press Enter to pass the turn to Player 1...");
                while (getchar() != '\n');
                getchar();
                currentPlayer = 1;
            }
        }
    }
}
