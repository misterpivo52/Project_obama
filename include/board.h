#ifndef BOARD_H
#define BOARD_H

#define FIELD_SIZE 10
#define MAX_SHIPS 10

typedef enum {
    WATER,
    SHIP,
    HIT,
    MISS
} CellType;

typedef struct {
    int size;
    int x[4], y[4];
    int hits;
} Ship;

typedef struct {
    char name[50];
    CellType field[FIELD_SIZE][FIELD_SIZE];
    CellType enemy_view[FIELD_SIZE][FIELD_SIZE];
    Ship ships[MAX_SHIPS];
    int ships_count;
    int ships_alive;
    int is_ai;
} Player;

typedef struct {
    int size;
    int count;
} ShipConfig;

void init_field(CellType field[FIELD_SIZE][FIELD_SIZE]);

int can_place_ship(CellType field[FIELD_SIZE][FIELD_SIZE], int x, int y, int size, int horizontal);

void place_ship(Player* player, int ship_index, int x, int y, int size, int horizontal);

void ai_place_ships(Player* player, ShipConfig* config, int config_count);

int check_hit(Player* target, Player* attacker, int x, int y);

void mark_destroyed_area(Player* target, Player* attacker, int ship_index);

#endif