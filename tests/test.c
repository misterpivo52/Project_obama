#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SDL_MAIN_HANDLED
#include "../include/board.h"

int tests_passed = 0;
int tests_failed = 0;

void test_result(const char* test_name, int passed) {
    if (passed) {
        printf("[PASS] %s\n", test_name);
        tests_passed++;
    } else {
        printf("[FAIL] %s\n", test_name);
        tests_failed++;
    }
}

void test_init_field() {
    CellType field[FIELD_SIZE][FIELD_SIZE];
    init_field(field);

    int all_water = 1;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (field[i][j] != WATER) {
                all_water = 0;
            }
        }
    }

    test_result("init_field", all_water);
}

void test_can_place_ship_valid() {
    CellType field[FIELD_SIZE][FIELD_SIZE];
    init_field(field);

    int result = can_place_ship(field, 0, 0, 4, 1);
    test_result("can_place_ship_valid", result == 1);
}

void test_can_place_ship_out_of_bounds() {
    CellType field[FIELD_SIZE][FIELD_SIZE];
    init_field(field);

    int result = can_place_ship(field, 9, 9, 4, 1);
    test_result("can_place_ship_out_of_bounds", result == 0);
}

void test_can_place_ship_collision() {
    CellType field[FIELD_SIZE][FIELD_SIZE];
    init_field(field);

    field[0][0] = SHIP;
    field[0][1] = SHIP;

    int result = can_place_ship(field, 1, 0, 2, 1);
    test_result("can_place_ship_collision", result == 0);
}

void test_place_ship_horizontal() {
    Player player;
    init_field(player.field);

    place_ship(&player, 0, 0, 0, 4, 1);

    int ok = 1;
    for (int i = 0; i < 4; i++) {
        if (player.field[0][i] != SHIP) ok = 0;
    }

    test_result("place_ship_horizontal", ok && player.ships[0].size == 4);
}

void test_place_ship_vertical() {
    Player player;
    init_field(player.field);

    place_ship(&player, 0, 2, 2, 3, 0);

    int ok = 1;
    for (int i = 0; i < 3; i++) {
        if (player.field[2 + i][2] != SHIP) ok = 0;
    }

    test_result("place_ship_vertical", ok);
}

void test_check_hit_miss() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);

    int result = check_hit(&target, &attacker, 0, 0);

    test_result("check_hit_miss", result == 0 && target.field[0][0] == MISS);
}

void test_check_hit_hit() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);
    target.ships_count = 1;

    place_ship(&target, 0, 0, 0, 3, 1);

    int result = check_hit(&target, &attacker, 0, 0);

    test_result("check_hit_hit", result == 1 && target.field[0][0] == HIT);
}

void test_check_hit_destroy() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);
    target.ships_count = 1;
    target.ships_alive = 1;

    place_ship(&target, 0, 0, 0, 2, 1);

    check_hit(&target, &attacker, 0, 0);
    int result = check_hit(&target, &attacker, 0, 1);

    test_result("check_hit_destroy", result == 2 && target.ships_alive == 0);
}

void test_mark_destroyed_area() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);

    place_ship(&target, 0, 2, 2, 2, 1);
    mark_destroyed_area(&target, &attacker, 0);

    int ok = (target.field[1][1] == MISS);

    test_result("mark_destroyed_area", ok);
}

void test_ai_place_ships_default() {
    Player player;
    init_field(player.field);

    ShipConfig config[4] = {
        {4, 1},
        {3, 1},
        {2, 1},
        {1, 1}
    };

    ai_place_ships(&player, config, 4);

    int ships_placed = 0;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (player.field[i][j] == SHIP) {
                ships_placed++;
            }
        }
    }

    test_result("ai_place_ships_default", ships_placed == 10 && player.ships_count == 4 && player.ships_alive == 4);
}

void test_ai_place_ships_custom() {
    Player player;
    init_field(player.field);

    ShipConfig config[2] = {
        {3, 2},
        {1, 3}
    };

    ai_place_ships(&player, config, 2);

    int ships_placed = 0;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (player.field[i][j] == SHIP) {
                ships_placed++;
            }
        }
    }

    test_result("ai_place_ships_custom", ships_placed == 9 && player.ships_count == 5 && player.ships_alive == 5);
}

void test_ai_place_ships_many() {
    Player player;
    init_field(player.field);

    ShipConfig config[4] = {
        {4, 2},
        {3, 2},
        {2, 2},
        {1, 2}
    };

    ai_place_ships(&player, config, 4);

    int ships_placed = 0;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (player.field[i][j] == SHIP) {
                ships_placed++;
            }
        }
    }

    test_result("ai_place_ships_many", ships_placed == 20 && player.ships_count == 8 && player.ships_alive == 8);
}

void test_ai_place_ships_only_small() {
    Player player;
    init_field(player.field);

    ShipConfig config[1] = {
        {1, 5}
    };

    ai_place_ships(&player, config, 1);

    int ships_placed = 0;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (player.field[i][j] == SHIP) {
                ships_placed++;
            }
        }
    }

    test_result("ai_place_ships_only_small", ships_placed == 5 && player.ships_count == 5);
}

void test_multiple_ships() {
    Player player;
    init_field(player.field);

    place_ship(&player, 0, 0, 0, 4, 1);
    place_ship(&player, 1, 2, 0, 3, 1);
    place_ship(&player, 2, 4, 0, 2, 1);
    place_ship(&player, 3, 6, 0, 1, 1);

    int ok = (player.ships[0].size == 4 &&
              player.ships[1].size == 3 &&
              player.ships[2].size == 2 &&
              player.ships[3].size == 1);

    test_result("multiple_ships", ok);
}

void test_ship_hits_tracking() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);
    target.ships_count = 1;

    place_ship(&target, 0, 0, 0, 3, 1);

    check_hit(&target, &attacker, 0, 0);
    check_hit(&target, &attacker, 0, 1);

    test_result("ship_hits_tracking", target.ships[0].hits == 2);
}

void test_boundary_corner() {
    CellType field[FIELD_SIZE][FIELD_SIZE];
    init_field(field);

    int result = can_place_ship(field, 0, 0, 1, 1);
    test_result("boundary_corner", result == 1);
}

void test_boundary_edge() {
    CellType field[FIELD_SIZE][FIELD_SIZE];
    init_field(field);

    int result = can_place_ship(field, 9, 6, 1, 1);
    test_result("boundary_edge", result == 1);
}

void test_ship_config_structure() {
    ShipConfig config[4];

    config[0].size = 4;
    config[0].count = 2;
    config[1].size = 3;
    config[1].count = 1;
    config[2].size = 2;
    config[2].count = 3;
    config[3].size = 1;
    config[3].count = 0;

    int total = config[0].count + config[1].count + config[2].count + config[3].count;

    test_result("ship_config_structure", total == 6);
}

void test_max_ships_limit() {
    Player player;
    init_field(player.field);
    player.ships_count = 0;

    for (int i = 0; i < MAX_SHIPS && i < 10; i++) {
        place_ship(&player, i, 0, i, 1, 1);
        player.ships_count++;
    }

    test_result("max_ships_limit", player.ships_count <= MAX_SHIPS);
}

void test_all_ships_destroyed() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);

    place_ship(&target, 0, 0, 0, 1, 1);
    place_ship(&target, 1, 2, 2, 1, 1);
    target.ships_count = 2;
    target.ships_alive = 2;

    check_hit(&target, &attacker, 0, 0);
    check_hit(&target, &attacker, 2, 2);

    test_result("all_ships_destroyed", target.ships_alive == 0);
}

void test_partial_ship_damage() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);
    target.ships_count = 1;
    target.ships_alive = 1;

    place_ship(&target, 0, 0, 0, 4, 1);

    check_hit(&target, &attacker, 0, 0);
    check_hit(&target, &attacker, 0, 1);

    test_result("partial_ship_damage", target.ships[0].hits == 2 && target.ships_alive == 1);
}


void place_ship_temp(CellType field[FIELD_SIZE][FIELD_SIZE], int x, int y, int size, int horizontal) {
    int dx = horizontal ? 0 : 1;
    int dy = horizontal ? 1 : 0;

    for (int i = 0; i < size; i++) {
        field[x + dx * i][y + dy * i] = SHIP;
    }
}

void test_ship_coordinates_tracking() {
    Player player;
    init_field(player.field);

    place_ship(&player, 0, 3, 5, 3, 0);

    int ok = (player.ships[0].x[0] == 3 && player.ships[0].y[0] == 5 &&
              player.ships[0].x[1] == 4 && player.ships[0].y[1] == 5 &&
              player.ships[0].x[2] == 5 && player.ships[0].y[2] == 5);

    test_result("ship_coordinates_tracking", ok);
}

void test_destroyed_area_marking() {
    Player target, attacker;
    init_field(target.field);
    init_field(attacker.field);
    target.ships_count = 1;
    target.ships_alive = 1;

    place_ship(&target, 0, 5, 5, 1, 1);

    check_hit(&target, &attacker, 5, 5);

    int surrounding_marked = 1;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int x = 5 + dx;
            int y = 5 + dy;
            if (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE) {
                if (x == 5 && y == 5) continue;
                if (target.field[x][y] != MISS) {
                    surrounding_marked = 0;
                }
            }
        }
    }

    test_result("destroyed_area_marking", surrounding_marked);
}

void test_complex_ship_config() {
    Player player;
    init_field(player.field);

    ShipConfig config[3] = {
        {4, 1},
        {2, 3},
        {1, 5}
    };

    ai_place_ships(&player, config, 3);

    int size_4_count = 0;
    int size_2_count = 0;
    int size_1_count = 0;

    for (int i = 0; i < player.ships_count; i++) {
        if (player.ships[i].size == 4) size_4_count++;
        if (player.ships[i].size == 2) size_2_count++;
        if (player.ships[i].size == 1) size_1_count++;
    }

    test_result("complex_ship_config", size_4_count == 1 && size_2_count == 3 && size_1_count == 5);
}

void test_empty_config() {
    Player player;
    init_field(player.field);

    ShipConfig config[1] = {{4, 0}};

    ai_place_ships(&player, config, 1);

    int ships_placed = 0;
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (player.field[i][j] == SHIP) ships_placed++;
        }
    }

    test_result("empty_config", ships_placed == 0 && player.ships_count == 0);
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    printf("\n=== BATTLESHIP TESTS ===\n\n");

    printf("--- Basic Field Tests ---\n");
    test_init_field();
    test_can_place_ship_valid();
    test_can_place_ship_out_of_bounds();
    test_can_place_ship_collision();
    test_boundary_corner();
    test_boundary_edge();

    printf("\n--- Ship Placement Tests ---\n");
    test_place_ship_horizontal();
    test_place_ship_vertical();
    test_multiple_ships();
    test_ship_coordinates_tracking();

    printf("\n--- Hit Detection Tests ---\n");
    test_check_hit_miss();
    test_check_hit_hit();
    test_check_hit_destroy();
    test_ship_hits_tracking();
    test_partial_ship_damage();
    test_all_ships_destroyed();

    printf("\n--- Area Marking Tests ---\n");
    test_mark_destroyed_area();
    test_destroyed_area_marking();

    printf("\n--- AI Placement Tests ---\n");
    test_ai_place_ships_default();
    test_ai_place_ships_custom();
    test_ai_place_ships_many();
    test_ai_place_ships_only_small();
    test_complex_ship_config();
    test_empty_config();

    printf("\n--- Configuration Tests ---\n");
    test_ship_config_structure();
    test_max_ships_limit();

    printf("\n=== RESULTS ===\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Total: %d\n", tests_passed + tests_failed);

    if (tests_failed == 0) {
        printf("\nALL TESTS PASSED!\n\n");
        return 0;
    } else {
        printf("\nSOME TESTS FAILED\n\n");
        return 1;
    }
}