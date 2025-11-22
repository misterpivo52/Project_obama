#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/gameplay.h"
#include "../include/board.h"

int main(void) {
    srand(time(NULL));

    printf("╔═══════════════════════════════════════╗\n");
    printf("║         BATTLESHIP GAME             ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    printf("\nSelect game mode:\n");
    printf("1 - Play against computer\n");
    printf("2 - Two players\n");
    printf("\nChoice: ");

    int gameMode;
    scanf("%d", &gameMode);

    if (gameMode == 2) {
        playTwoPlayers();
    } else {
        playAgainstComputer();
    }

    printf("\nGame over!\n");
    return 0;
}
