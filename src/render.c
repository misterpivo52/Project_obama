#include <stdio.h>
#include <string.h>
#include "../include/render.h"

SDL_Color COLOR_WATER = {135, 206, 250, 255};
SDL_Color COLOR_SHIP = {100, 200, 100, 255};
SDL_Color COLOR_HIT = {220, 20, 60, 255};
SDL_Color COLOR_MISS = {255, 215, 0, 255};
SDL_Color COLOR_GRID = {70, 130, 180, 255};
SDL_Color COLOR_BG = {30, 30, 60, 255};
SDL_Color COLOR_WHITE = {255, 255, 255, 255};
SDL_Color COLOR_BUTTON = {100, 149, 237, 255};
SDL_Color COLOR_BUTTON_HOVER = {135, 175, 255, 255};
SDL_Color COLOR_GREEN = {0, 255, 100, 255};

void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_text_centered(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {(WINDOW_WIDTH - surface->w) / 2, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_button(SDL_Renderer* renderer, TTF_Font* font, Button* btn) {
    SDL_Color color = btn->is_hovered ? COLOR_BUTTON_HOVER : COLOR_BUTTON;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &btn->rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &btn->rect);

    SDL_Surface* surface = TTF_RenderText_Blended(font, btn->text, COLOR_WHITE);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect text_rect = {
        btn->rect.x + (btn->rect.w - surface->w) / 2,
        btn->rect.y + (btn->rect.h - surface->h) / 2,
        surface->w, surface->h
    };
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_field(SDL_Renderer* renderer, TTF_Font* font, CellType field[FIELD_SIZE][FIELD_SIZE],
                int offset_x, int offset_y, int show_ships, int interactive, int mouse_x, int mouse_y) {
    char title[50];
    sprintf(title, show_ships ? "Your Field" : "Enemy Field");
    draw_text(renderer, font, title, offset_x, offset_y - 30, COLOR_WHITE);

    for (int i = 0; i < FIELD_SIZE; i++) {
        char label[3];
        sprintf(label, "%d", i);
        draw_text(renderer, font, label, offset_x - 25, offset_y + i * CELL_SIZE + 10, COLOR_WHITE);
        draw_text(renderer, font, label, offset_x + i * CELL_SIZE + 15, offset_y - 25, COLOR_WHITE);
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            SDL_Rect rect = {
                offset_x + j * CELL_SIZE,
                offset_y + i * CELL_SIZE,
                CELL_SIZE - 2,
                CELL_SIZE - 2
            };

            if (interactive && mouse_x >= rect.x && mouse_x < rect.x + rect.w &&
                mouse_y >= rect.y && mouse_y < rect.y + rect.h) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 150, 255);
                SDL_RenderFillRect(renderer, &rect);
            }

            CellType cell = field[i][j];
            SDL_Color color;

            if (cell == WATER) {
                color = COLOR_WATER;
            } else if (cell == SHIP) {
                color = show_ships ? COLOR_SHIP : COLOR_WATER;
            } else if (cell == HIT) {
                color = COLOR_HIT;
            } else {
                color = COLOR_MISS;
            }

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);

            SDL_SetRenderDrawColor(renderer, COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void draw_ship_preview(SDL_Renderer* renderer, int offset_x, int offset_y,
                       int x, int y, int size, int horizontal, int valid) {
    SDL_Color color = valid ? (SDL_Color){100, 255, 100, 150} : (SDL_Color){255, 100, 100, 150};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < size; i++) {
        int draw_x = horizontal ? x : x + i;
        int draw_y = horizontal ? y + i : y;

        if (draw_x >= 0 && draw_x < FIELD_SIZE && draw_y >= 0 && draw_y < FIELD_SIZE) {
            SDL_Rect rect = {
                offset_x + draw_y * CELL_SIZE,
                offset_y + draw_x * CELL_SIZE,
                CELL_SIZE - 2,
                CELL_SIZE - 2
            };
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void render_menu(Game* game, int mouse_x, int mouse_y) {
    SDL_SetRenderDrawColor(game->renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 255);
    SDL_RenderClear(game->renderer);

    draw_text_centered(game->renderer, game->font, "BATTLESHIP", 100, COLOR_WHITE);
    draw_text_centered(game->renderer, game->font_small, "Naval Combat Game", 160, COLOR_WHITE);

    Button buttons[3];

    buttons[0].rect = (SDL_Rect){WINDOW_WIDTH/2 - 150, 250, 300, 60};
    strcpy(buttons[0].text, "Player vs Player");
    buttons[0].is_hovered = (mouse_x >= buttons[0].rect.x && mouse_x < buttons[0].rect.x + buttons[0].rect.w &&
                              mouse_y >= buttons[0].rect.y && mouse_y < buttons[0].rect.y + buttons[0].rect.h);

    buttons[1].rect = (SDL_Rect){WINDOW_WIDTH/2 - 150, 330, 300, 60};
    strcpy(buttons[1].text, "Player vs Computer");
    buttons[1].is_hovered = (mouse_x >= buttons[1].rect.x && mouse_x < buttons[1].rect.x + buttons[1].rect.w &&
                              mouse_y >= buttons[1].rect.y && mouse_y < buttons[1].rect.y + buttons[1].rect.h);

    buttons[2].rect = (SDL_Rect){WINDOW_WIDTH/2 - 150, 410, 300, 60};
    strcpy(buttons[2].text, "Exit");
    buttons[2].is_hovered = (mouse_x >= buttons[2].rect.x && mouse_x < buttons[2].rect.x + buttons[2].rect.w &&
                              mouse_y >= buttons[2].rect.y && mouse_y < buttons[2].rect.y + buttons[2].rect.h);

    for (int i = 0; i < 3; i++) {
        draw_button(game->renderer, game->font_small, &buttons[i]);
    }

    draw_text_centered(game->renderer, game->font_small, "Click to place ships | R to rotate", 550, COLOR_WHITE);
}

void render_config(Game* game, int mouse_x, int mouse_y) {
    SDL_SetRenderDrawColor(game->renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 255);
    SDL_RenderClear(game->renderer);

    draw_text_centered(game->renderer, game->font, "Configure Ships", 30, COLOR_WHITE);
    draw_text_centered(game->renderer, game->font_small, "Choose number of ships for each type", 80, COLOR_WHITE);

    char* ship_names[] = {"Battleship (4 cells)", "Cruiser (3 cells)", "Destroyer (2 cells)", "Submarine (1 cell)"};

    for (int i = 0; i < 4; i++) {
        int btn_y = 200 + i * 80;

        draw_text_centered(game->renderer, game->font_small, ship_names[i], btn_y - 30, COLOR_WHITE);

        Button minus_btn;
        minus_btn.rect = (SDL_Rect){WINDOW_WIDTH/2 - 200, btn_y, 80, 60};
        strcpy(minus_btn.text, "-");
        minus_btn.is_hovered = (mouse_x >= minus_btn.rect.x && mouse_x < minus_btn.rect.x + minus_btn.rect.w &&
                                mouse_y >= minus_btn.rect.y && mouse_y < minus_btn.rect.y + minus_btn.rect.h);
        draw_button(game->renderer, game->font, &minus_btn);

        char count_str[10];
        sprintf(count_str, "%d", game->ship_config[i].count);
        draw_text_centered(game->renderer, game->font, count_str, btn_y + 15, COLOR_WHITE);

        Button plus_btn;
        plus_btn.rect = (SDL_Rect){WINDOW_WIDTH/2 + 100, btn_y, 80, 60};
        strcpy(plus_btn.text, "+");
        plus_btn.is_hovered = (mouse_x >= plus_btn.rect.x && mouse_x < plus_btn.rect.x + plus_btn.rect.w &&
                               mouse_y >= plus_btn.rect.y && mouse_y < plus_btn.rect.y + plus_btn.rect.h);
        draw_button(game->renderer, game->font, &plus_btn);
    }

    char total_str[50];
    sprintf(total_str, "Total ships: %d", game->total_ships);
    draw_text_centered(game->renderer, game->font_small, total_str, 520, COLOR_WHITE);

    Button start_btn;
    start_btn.rect = (SDL_Rect){WINDOW_WIDTH/2 - 150, 550, 300, 60};
    strcpy(start_btn.text, game->total_ships > 0 ? "Start Game" : "Select ships first");
    start_btn.is_hovered = (mouse_x >= start_btn.rect.x && mouse_x < start_btn.rect.x + start_btn.rect.w &&
                            mouse_y >= start_btn.rect.y && mouse_y < start_btn.rect.y + start_btn.rect.h);
    draw_button(game->renderer, game->font_small, &start_btn);
}

void render_setup(Game* game, int mouse_x, int mouse_y) {
    SDL_SetRenderDrawColor(game->renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 255);
    SDL_RenderClear(game->renderer);

    Player* player = (game->state == STATE_SETUP_P1) ? &game->player1 : &game->player2;

    char title[100];
    sprintf(title, "%s - Place your ships", player->name);
    draw_text_centered(game->renderer, game->font, title, 30, COLOR_WHITE);

    if (game->current_ship < game->total_ships) {
        int current_size = 0;
        int ships_placed = 0;

        for (int cfg = 0; cfg < game->config_count; cfg++) {
            if (game->current_ship < ships_placed + game->ship_config[cfg].count) {
                current_size = game->ship_config[cfg].size;
                break;
            }
            ships_placed += game->ship_config[cfg].count;
        }

        char info[100];
        sprintf(info, "Placing ship %d/%d (size: %d) | Press R to rotate | Press SPACE for random",
                game->current_ship + 1, game->total_ships, current_size);
        draw_text_centered(game->renderer, game->font_small, info, 80, COLOR_WHITE);
    }

    Button random_btn;
    random_btn.rect = (SDL_Rect){WINDOW_WIDTH/2 - 150, 600, 300, 50};
    strcpy(random_btn.text, "Random Placement (SPACE)");
    random_btn.is_hovered = (mouse_x >= random_btn.rect.x && mouse_x < random_btn.rect.x + random_btn.rect.w &&
                              mouse_y >= random_btn.rect.y && mouse_y < random_btn.rect.y + random_btn.rect.h);
    draw_button(game->renderer, game->font_small, &random_btn);

    int offset_x = WINDOW_WIDTH / 2 - (FIELD_SIZE * CELL_SIZE) / 2;
    int offset_y = 150;

    draw_field(game->renderer, game->font_small, player->field, offset_x, offset_y, 1, 1, mouse_x, mouse_y);

    if (game->current_ship < game->total_ships) {
        int cell_x = (mouse_y - offset_y) / CELL_SIZE;
        int cell_y = (mouse_x - offset_x) / CELL_SIZE;

        if (cell_x >= 0 && cell_x < FIELD_SIZE && cell_y >= 0 && cell_y < FIELD_SIZE) {
            int current_size = 0;
            int ships_placed = 0;

            for (int cfg = 0; cfg < game->config_count; cfg++) {
                if (game->current_ship < ships_placed + game->ship_config[cfg].count) {
                    current_size = game->ship_config[cfg].size;
                    break;
                }
                ships_placed += game->ship_config[cfg].count;
            }

            int valid = can_place_ship(player->field, cell_x, cell_y, current_size, !game->ship_direction);
            draw_ship_preview(game->renderer, offset_x, offset_y, cell_x, cell_y,
                            current_size, !game->ship_direction, valid);
        }
    }
}

void render_game(Game* game, int mouse_x, int mouse_y) {
    SDL_SetRenderDrawColor(game->renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 255);
    SDL_RenderClear(game->renderer);

    char turn[100];
    sprintf(turn, "%s's Turn", game->current_player->name);
    draw_text_centered(game->renderer, game->font, turn, 20, COLOR_WHITE);

    if (game->last_result == 1) {
        draw_text_centered(game->renderer, game->font_small, "HIT!", 60, COLOR_HIT);
    } else if (game->last_result == 2) {
        draw_text_centered(game->renderer, game->font_small, "SHIP DESTROYED!", 60, COLOR_HIT);
    } else if (game->last_result == 0) {
        draw_text_centered(game->renderer, game->font_small, "MISS!", 60, COLOR_MISS);
    }

    int offset_y = 120;

    int show_p1_ships = (game->mode == MODE_PVC) ? 1 : 0;
    int show_p2_ships = 0;

    draw_field(game->renderer, game->font_small, game->player1.field,
               50, offset_y, show_p1_ships, 0, mouse_x, mouse_y);

    draw_field(game->renderer, game->font_small, game->player2.field,
               650, offset_y, show_p2_ships, 0, mouse_x, mouse_y);

    if (game->current_player == &game->player1 && !game->current_player->is_ai) {
        draw_text(game->renderer, game->font_small, "YOUR SHIPS", 50, offset_y - 30, COLOR_WHITE);
        draw_text(game->renderer, game->font_small, "ATTACK HERE", 850, offset_y - 30, COLOR_GREEN);
    } else if (game->current_player == &game->player2 && !game->current_player->is_ai) {
        draw_text(game->renderer, game->font_small, "ATTACK HERE", 50, offset_y - 30, COLOR_GREEN);
        draw_text(game->renderer, game->font_small, "YOUR SHIPS", 850, offset_y - 30, COLOR_WHITE);
    }

    char ships_left[100];
    sprintf(ships_left, "Player 1 ships: %d | Player 2 ships: %d",
            game->player1.ships_alive, game->player2.ships_alive);
    draw_text_centered(game->renderer, game->font_small, ships_left, 650, COLOR_WHITE);
}

void render_game_over(Game* game, int mouse_x, int mouse_y) {
    SDL_SetRenderDrawColor(game->renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 255);
    SDL_RenderClear(game->renderer);

    draw_text_centered(game->renderer, game->font, "GAME OVER", 100, COLOR_WHITE);

    char winner[100];
    SDL_Color green = {0, 255, 0, 255};
    if (game->player1.ships_alive == 0) {
        sprintf(winner, "Winner: %s!", game->player2.name);
    } else {
        sprintf(winner, "Winner: %s!", game->player1.name);
    }
    draw_text_centered(game->renderer, game->font, winner, 180, green);

    Button btn;
    btn.rect = (SDL_Rect){WINDOW_WIDTH/2 - 150, 300, 300, 60};
    strcpy(btn.text, "Back to Menu");
    btn.is_hovered = (mouse_x >= btn.rect.x && mouse_x < btn.rect.x + btn.rect.w &&
                      mouse_y >= btn.rect.y && mouse_y < btn.rect.y + btn.rect.h);
    draw_button(game->renderer, game->font_small, &btn);
}