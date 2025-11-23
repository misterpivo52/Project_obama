#include <string.h>
#include <stdlib.h>
#include "../include/game.h"

void ai_turn(Game* game) {
    int x, y;
    int found = 0;

    for (int i = 0; i < FIELD_SIZE && !found; i++) {
        for (int j = 0; j < FIELD_SIZE && !found; j++) {
            if (game->player1.field[i][j] == HIT) {
                int dirs[][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                for (int d = 0; d < 4 && !found; d++) {
                    x = i + dirs[d][0];
                    y = j + dirs[d][1];
                    if (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE &&
                        game->player1.field[x][y] != HIT && game->player1.field[x][y] != MISS) {
                        found = 1;
                    }
                }
            }
        }
    }

    if (!found) {
        do {
            x = rand() % FIELD_SIZE;
            y = rand() % FIELD_SIZE;
        } while (game->player1.field[x][y] == HIT || game->player1.field[x][y] == MISS);
    }

    game->last_result = check_hit(&game->player1, &game->player2, x, y);

    if (game->last_result == 0) {
        game->current_player = &game->player1;
        game->target_player = &game->player2;
    }
}

void handle_menu_click(Game* game, int x, int y) {
    if (x >= WINDOW_WIDTH/2 - 150 && x < WINDOW_WIDTH/2 + 150) {
        if (y >= 250 && y < 310) {
            game->mode = MODE_PVP;
            strcpy(game->player1.name, "Player 1");
            strcpy(game->player2.name, "Player 2");
            game->player1.is_ai = 0;
            game->player2.is_ai = 0;
            game->state = STATE_CONFIG;
        } else if (y >= 330 && y < 390) {
            game->mode = MODE_PVC;
            strcpy(game->player1.name, "Player");
            strcpy(game->player2.name, "Computer");
            game->player1.is_ai = 0;
            game->player2.is_ai = 1;
            game->state = STATE_CONFIG;
        } else if (y >= 410 && y < 470) {
            exit(0);
        }
    }
}

void handle_config_click(Game* game, int x, int y) {
    int ship_sizes[] = {4, 3, 2, 1};

    for (int i = 0; i < 4; i++) {
        int btn_y = 200 + i * 80;

        if (y >= btn_y && y < btn_y + 60) {
            if (x >= WINDOW_WIDTH/2 - 200 && x < WINDOW_WIDTH/2 - 100) {
                if (game->ship_config[i].count > 0) {
                    game->ship_config[i].count--;
                    game->total_ships--;
                }
            }
            else if (x >= WINDOW_WIDTH/2 + 100 && x < WINDOW_WIDTH/2 + 200) {
                if (game->ship_config[i].count < 5) {
                    game->ship_config[i].count++;
                    game->total_ships++;
                }
            }
        }
    }

    if (x >= WINDOW_WIDTH/2 - 150 && x < WINDOW_WIDTH/2 + 150 &&
        y >= 550 && y < 610 && game->total_ships > 0) {

        int cfg_count = 0;
        for (int i = 0; i < 4; i++) {
            if (game->ship_config[i].count > 0) {
                game->ship_config[cfg_count].size = ship_sizes[i];
                game->ship_config[cfg_count].count = game->ship_config[i].count;
                cfg_count++;
            }
        }
        game->config_count = cfg_count;

        init_field(game->player1.field);
        init_field(game->player1.enemy_view);
        init_field(game->player2.field);
        init_field(game->player2.enemy_view);

        if (game->mode == MODE_PVC) {
            ai_place_ships(&game->player2, game->ship_config, game->config_count);
        }

        game->state = STATE_SETUP_P1;
        game->current_ship = 0;
        game->ship_direction = 0;
    }
}

void handle_setup_click(Game* game, int x, int y) {
    Player* player = (game->state == STATE_SETUP_P1) ? &game->player1 : &game->player2;

    if (x >= WINDOW_WIDTH/2 - 150 && x < WINDOW_WIDTH/2 + 150 &&
        y >= 600 && y < 650) {
        init_field(player->field);
        ai_place_ships(player, game->ship_config, game->config_count);
        player->ships_count = game->total_ships;
        game->current_ship = game->total_ships;

        if (game->state == STATE_SETUP_P1 && game->mode == MODE_PVP) {
            game->state = STATE_SETUP_P2;
            game->current_ship = 0;
        } else {
            game->state = STATE_GAME;
            game->current_player = &game->player1;
            game->target_player = &game->player2;
            game->last_result = -1;
        }
        return;
    }

    int offset_x = WINDOW_WIDTH / 2 - (FIELD_SIZE * CELL_SIZE) / 2;
    int offset_y = 150;

    int cell_x = (y - offset_y) / CELL_SIZE;
    int cell_y = (x - offset_x) / CELL_SIZE;

    if (cell_x >= 0 && cell_x < FIELD_SIZE && cell_y >= 0 && cell_y < FIELD_SIZE) {
        int current_size = 0;
        int ships_placed_of_size = 0;

        for (int cfg = 0; cfg < game->config_count; cfg++) {
            int count_needed = game->ship_config[cfg].count;
            if (game->current_ship < ships_placed_of_size + count_needed) {
                current_size = game->ship_config[cfg].size;
                break;
            }
            ships_placed_of_size += count_needed;
        }

        if (can_place_ship(player->field, cell_x, cell_y, current_size, !game->ship_direction)) {
            place_ship(player, game->current_ship, cell_x, cell_y, current_size, !game->ship_direction);
            game->current_ship++;

            if (game->current_ship >= game->total_ships) {
                player->ships_count = game->total_ships;
                player->ships_alive = game->total_ships;

                if (game->state == STATE_SETUP_P1 && game->mode == MODE_PVP) {
                    game->state = STATE_SETUP_P2;
                    game->current_ship = 0;
                } else {
                    game->state = STATE_GAME;
                    game->current_player = &game->player1;
                    game->target_player = &game->player2;
                    game->last_result = -1;
                }
            }
        }
    }
}

void handle_game_click(Game* game, int x, int y) {
    if (game->current_player->is_ai) return;

    int offset_y = 120;
    int cell_x, cell_y;

    if (game->current_player == &game->player1) {
        int offset_x = 650;
        cell_x = (y - offset_y) / CELL_SIZE;
        cell_y = (x - offset_x) / CELL_SIZE;

        if (cell_x >= 0 && cell_x < FIELD_SIZE && cell_y >= 0 && cell_y < FIELD_SIZE) {
            if (game->player2.field[cell_x][cell_y] != HIT && game->player2.field[cell_x][cell_y] != MISS) {
                game->last_result = check_hit(&game->player2, &game->player1, cell_x, cell_y);

                if (game->player2.ships_alive == 0) {
                    game->state = STATE_GAME_OVER;
                    return;
                }

                if (game->last_result == 0) {
                    game->current_player = &game->player2;
                    game->target_player = &game->player1;
                }
            }
        }
    }
    else if (game->current_player == &game->player2) {
        int offset_x = 50;
        cell_x = (y - offset_y) / CELL_SIZE;
        cell_y = (x - offset_x) / CELL_SIZE;

        if (cell_x >= 0 && cell_x < FIELD_SIZE && cell_y >= 0 && cell_y < FIELD_SIZE) {
            if (game->player1.field[cell_x][cell_y] != HIT && game->player1.field[cell_x][cell_y] != MISS) {
                game->last_result = check_hit(&game->player1, &game->player2, cell_x, cell_y);

                if (game->player1.ships_alive == 0) {
                    game->state = STATE_GAME_OVER;
                    return;
                }

                if (game->last_result == 0) {
                    game->current_player = &game->player1;
                    game->target_player = &game->player2;
                }
            }
        }
    }
}