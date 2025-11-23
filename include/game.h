#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700
#define CELL_SIZE 40

typedef enum {
    STATE_MENU,
    STATE_SETUP_P1,
    STATE_SETUP_P2,
    STATE_GAME,
    STATE_GAME_OVER
} GameState;

typedef enum {
    MODE_PVP,
    MODE_PVC
} GameMode;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* font_small;
    GameState state;
    GameMode mode;
    Player player1;
    Player player2;
    Player* current_player;
    Player* target_player;
    int current_ship;
    int ship_direction;
    int selected_x, selected_y;
    int last_result;
} Game;

void init_game(Game* game);
void cleanup_game(Game* game);
void handle_menu_click(Game* game, int x, int y);
void handle_setup_click(Game* game, int x, int y);
void handle_game_click(Game* game, int x, int y);
void ai_turn(Game* game);

#endif