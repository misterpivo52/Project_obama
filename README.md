# Sea Battle

## Team
- Makohon Vladyslav
- Tymoshyn Mykhailo
- Borovyk Bohdan

## Overview

Sea Battle is a classic Battleship implementation that supports head-to-head play or a match against a bot. The project is split into board logic, game flow, and SDL2 rendering modules, plus a focused unit-test suite for the core mechanics.

## Requirements

- C99-capable compiler (`gcc` or `clang`) and `make`.
- `pkg-config` so the build can discover SDL headers and libraries.
- SDL2 runtime and development headers, plus `SDL2_ttf`.

### Installing dependencies

- **macOS (Homebrew)**  
  `brew install sdl2 sdl2_ttf pkg-config`

- **Ubuntu/Debian**  
  ```
  sudo apt update
  sudo apt install build-essential pkg-config libsdl2-dev libsdl2-ttf-dev
  ```

- **Arch Linux**  
  `sudo pacman -S base-devel pkgconf sdl2 sdl2_ttf`

- **Windows (WSL)**  
  Install [Windows Subsystem for Linux](https://learn.microsoft.com/windows/wsl/install) with an Ubuntu/Debian distro, then follow the Linux instructions inside WSL. Run `make` and the resulting binaries from the WSL terminal.

If `pkg-config` cannot find SDL, set `PKG_CONFIG_PATH=<path>/lib/pkgconfig` before building.

## Build and run

```bash
make
./bin/battleship
```

- The game binary lands in `bin/battleship`.
- `make clean` removes `bin/` and `obj/`.
- Run commands from the repository root.

## Tests

Unit tests live in `tests/test.c`. They cover field initialization, ship placement validation, hit detection, and AI placement logic.

```bash
make test
```

Tests build into `bin/test`. The runner prints grouped sections and exits with a non-zero code if anything fails, making it CI-friendly.

## Gameplay flow

### Initialization

* Two boards are created.
* Select either **1 vs bot** or **2 players**.
* Ships are placed automatically or manually.

### Ship placement

Each ship is validated before landing on the board:

* Random coordinates and orientation (horizontal/vertical) are chosen.
* Checks ensure the ship:

  * stays within the board;
  * does not overlap other ships;
  * keeps a one-cell buffer to neighboring ships.
* When all checks pass, the ship is committed.

## Game loop

Play continues until one player eliminates every enemy ship.

### Player turn

* Both boards are rendered.
* The player enters shot coordinates.
* The outcome is evaluated:

  * **Hit** — the player shoots again.
  * **Miss** — the turn passes to the opponent.
* After every shot the win condition is checked.

### Computer turn (vs bot)

* The AI selects a target:

  * follows up around the last hit if available;
  * otherwise picks a random cell.
* After shooting it checks whether the game has ended.

## Bot algorithm

* With a stored hit it probes the four adjacent cells and picks an untouched one.
* With no hits it fires randomly.
* After each shot:

  * hits are recorded for follow-up;
  * misses clear the stored state.

## Victory check

The opponent’s board is scanned:

* If any intact segment remains, the battle continues.
* If all ships are destroyed, the shooter wins.

## Project layout

- `src/` — gameplay source files (`main.c`, board, game, render modules).
- `include/` — shared headers containing structs and public APIs.
- `tests/` — console test suite (`test.c`).
- `makefile` — targets `all`, `test`, `clean` for builds and verification.
