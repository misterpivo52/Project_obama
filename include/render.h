#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"

typedef struct {
    SDL_Rect rect;
    char text[50];
    int is_hovered;
} Button;

void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color);
void draw_text_centered(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, SDL_Color color);
void draw_button(SDL_Renderer* renderer, TTF_Font* font, Button* btn);
void draw_field(SDL_Renderer* renderer, TTF_Font* font, CellType field[FIELD_SIZE][FIELD_SIZE],
                int offset_x, int offset_y, int show_ships, int interactive, int mouse_x, int mouse_y);
void draw_ship_preview(SDL_Renderer* renderer, int offset_x, int offset_y,
                       int x, int y, int size, int horizontal, int valid);

void render_menu(Game* game, int mouse_x, int mouse_y);
void render_setup(Game* game, int mouse_x, int mouse_y);
void render_game(Game* game, int mouse_x, int mouse_y);
void render_game_over(Game* game, int mouse_x, int mouse_y);

#endif