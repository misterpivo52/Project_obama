#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/game.h"
#include "../include/render.h"

void init_game(Game* game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0) {
        printf("TTF Error: %s\n", TTF_GetError());
        exit(1);
    }

    game->window = SDL_CreateWindow("Battleship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    const char* font_paths[] = {
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/verdana.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "arial.ttf"
    };

    game->font = NULL;
    game->font_small = NULL;

    for (int i = 0; i < 6; i++) {
        game->font = TTF_OpenFont(font_paths[i], 36);
        if (game->font) {
            game->font_small = TTF_OpenFont(font_paths[i], 18);
            break;
        }
    }

    if (!game->font || !game->font_small) {
        printf("Font loading failed.\n");
        exit(1);
    }

    game->state = STATE_MENU;

    game->ship_config[0].size = 4;
    game->ship_config[0].count = 1;
    game->ship_config[1].size = 3;
    game->ship_config[1].count = 1;
    game->ship_config[2].size = 2;
    game->ship_config[2].count = 1;
    game->ship_config[3].size = 1;
    game->ship_config[3].count = 1;
    game->total_ships = 4;
}

void cleanup_game(Game* game) {
    TTF_CloseFont(game->font);
    TTF_CloseFont(game->font_small);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    srand(time(NULL));

    Game game;
    init_game(&game);

    int running = 1;
    int mouse_x = 0, mouse_y = 0;
    Uint32 last_ai_time = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEMOTION) {
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (game.state == STATE_MENU) {
                        handle_menu_click(&game, mouse_x, mouse_y);
                    } else if (game.state == STATE_CONFIG) {
                        handle_config_click(&game, mouse_x, mouse_y);
                    } else if (game.state == STATE_SETUP_P1 || game.state == STATE_SETUP_P2) {
                        handle_setup_click(&game, mouse_x, mouse_y);
                    } else if (game.state == STATE_GAME) {
                        handle_game_click(&game, mouse_x, mouse_y);
                    } else if (game.state == STATE_GAME_OVER) {
                        if (mouse_x >= WINDOW_WIDTH/2 - 150 && mouse_x < WINDOW_WIDTH/2 + 150 &&
                            mouse_y >= 300 && mouse_y < 360) {
                            game.state = STATE_MENU;

                            game.ship_config[0].count = 1;
                            game.ship_config[1].count = 1;
                            game.ship_config[2].count = 1;
                            game.ship_config[3].count = 1;
                            game.total_ships = 4;
                        }
                    }
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_r) {
                    if (game.state == STATE_SETUP_P1 || game.state == STATE_SETUP_P2) {
                        game.ship_direction = !game.ship_direction;
                    }
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    if (game.state == STATE_SETUP_P1 || game.state == STATE_SETUP_P2) {
                        Player* player = (game.state == STATE_SETUP_P1) ? &game.player1 : &game.player2;
                        init_field(player->field);
                        ai_place_ships(player, game.ship_config, game.config_count);
                        player->ships_count = game.total_ships;
                        game.current_ship = game.total_ships;

                        if (game.state == STATE_SETUP_P1 && game.mode == MODE_PVP) {
                            game.state = STATE_SETUP_P2;
                            game.current_ship = 0;
                        } else {
                            game.state = STATE_GAME;
                            game.current_player = &game.player1;
                            game.target_player = &game.player2;
                            game.last_result = -1;
                        }
                    }
                }
            }
        }

        if (game.state == STATE_GAME && game.current_player->is_ai) {
            Uint32 current_time = SDL_GetTicks();
            if (current_time - last_ai_time > 1000) {
                ai_turn(&game);
                last_ai_time = current_time;

                if (game.target_player->ships_alive == 0) {
                    game.state = STATE_GAME_OVER;
                }
            }
        }

        if (game.state == STATE_MENU) {
            render_menu(&game, mouse_x, mouse_y);
        } else if (game.state == STATE_CONFIG) {
            render_config(&game, mouse_x, mouse_y);
        } else if (game.state == STATE_SETUP_P1 || game.state == STATE_SETUP_P2) {
            render_setup(&game, mouse_x, mouse_y);
        } else if (game.state == STATE_GAME) {
            render_game(&game, mouse_x, mouse_y);
        } else if (game.state == STATE_GAME_OVER) {
            render_game_over(&game, mouse_x, mouse_y);
        }

        SDL_RenderPresent(game.renderer);
        SDL_Delay(16);
    }

    cleanup_game(&game);
    return 0;
}