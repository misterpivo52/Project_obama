CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude `pkg-config --cflags sdl2`
CFLAGS_TEST = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = `pkg-config --libs sdl2` -lSDL2_ttf

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/board.c $(SRC_DIR)/game.c $(SRC_DIR)/render.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/board.o $(OBJ_DIR)/game.o $(OBJ_DIR)/render.o

BOARD_TEST_SOURCES = $(SRC_DIR)/board.c $(TEST_DIR)/test.c
BOARD_TEST_OBJECTS = $(OBJ_DIR)/board.o $(OBJ_DIR)/test.o

TARGET = $(BIN_DIR)/battleship
TEST_TARGET = $(BIN_DIR)/test

.PHONY: all clean test run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test.o: $(TEST_DIR)/test.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

test: $(TEST_TARGET)
	@echo "Running tests..."
	@./$(TEST_TARGET)

$(TEST_TARGET): $(BOARD_TEST_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS_TEST) -o $@ $^

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)