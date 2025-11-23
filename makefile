CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude `pkg-config --cflags sdl2`
LDFLAGS = `pkg-config --libs sdl2` -lSDL2_ttf

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/board.c $(SRC_DIR)/game.c $(SRC_DIR)/render.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/board.o $(OBJ_DIR)/game.o $(OBJ_DIR)/render.o
TARGET = $(BIN_DIR)/battleship

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)