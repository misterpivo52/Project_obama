#include <stdlib.h>
#include "../include/board.h"

void init_field(CellType field[FIELD_SIZE][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            field[i][j] = WATER;
        }
    }
}

int can_place_ship(CellType field[FIELD_SIZE][FIELD_SIZE], int x, int y, int size, int horizontal) {
    int dx = horizontal ? 0 : 1;
    int dy = horizontal ? 1 : 0;

    if (x + dx * (size - 1) >= FIELD_SIZE || y + dy * (size - 1) >= FIELD_SIZE) {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        int curr_x = x + dx * i;
        int curr_y = y + dy * i;

        for (int check_x = curr_x - 1; check_x <= curr_x + 1; check_x++) {
            for (int check_y = curr_y - 1; check_y <= curr_y + 1; check_y++) {
                if (check_x >= 0 && check_x < FIELD_SIZE &&
                    check_y >= 0 && check_y < FIELD_SIZE &&
                    field[check_x][check_y] == SHIP) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

void place_ship(Player* player, int ship_index, int x, int y, int size, int horizontal) {
    int dx = horizontal ? 0 : 1;
    int dy = horizontal ? 1 : 0;

    player->ships[ship_index].size = size;
    player->ships[ship_index].hits = 0;

    for (int i = 0; i < size; i++) {
        int curr_x = x + dx * i;
        int curr_y = y + dy * i;
        player->field[curr_x][curr_y] = SHIP;
        player->ships[ship_index].x[i] = curr_x;
        player->ships[ship_index].y[i] = curr_y;
    }
}

void ai_place_ships(Player* player, ShipConfig* config, int config_count) {
    int ship_index = 0;

    for (int cfg = 0; cfg < config_count; cfg++) {
        for (int ship_num = 0; ship_num < config[cfg].count; ship_num++) {
            int placed = 0;
            while (!placed) {
                int x = rand() % FIELD_SIZE;
                int y = rand() % FIELD_SIZE;
                int horizontal = rand() % 2;

                if (can_place_ship(player->field, x, y, config[cfg].size, horizontal)) {
                    place_ship(player, ship_index, x, y, config[cfg].size, horizontal);
                    ship_index++;
                    placed = 1;
                }
            }
        }
    }
    player->ships_count = ship_index;
    player->ships_alive = ship_index;
}

void mark_destroyed_area(Player* target, Player* attacker, int ship_index) {
    (void)attacker;
    Ship* ship = &target->ships[ship_index];

    for (int i = 0; i < ship->size; i++) {
        int ship_x = ship->x[i];
        int ship_y = ship->y[i];

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int check_x = ship_x + dx;
                int check_y = ship_y + dy;

                if (check_x >= 0 && check_x < FIELD_SIZE &&
                    check_y >= 0 && check_y < FIELD_SIZE &&
                    target->field[check_x][check_y] == WATER) {
                    target->field[check_x][check_y] = MISS;
                }
            }
        }
    }
}

int check_hit(Player* target, Player* attacker, int x, int y) {
    if (target->field[x][y] == SHIP) {
        target->field[x][y] = HIT;

        for (int i = 0; i < target->ships_count; i++) {
            for (int j = 0; j < target->ships[i].size; j++) {
                if (target->ships[i].x[j] == x && target->ships[i].y[j] == y) {
                    target->ships[i].hits++;

                    if (target->ships[i].hits == target->ships[i].size) {
                        target->ships_alive--;
                        mark_destroyed_area(target, attacker, i);
                        return 2;
                    }
                    return 1;
                }
            }
        }
    }

    target->field[x][y] = MISS;
    return 0;
}