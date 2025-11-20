CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/board.c $(SRC_DIR)/placement.c $(SRC_DIR)/gameplay.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/board.o $(OBJ_DIR)/placement.o $(OBJ_DIR)/gameplay.o
TARGET = battleship

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
