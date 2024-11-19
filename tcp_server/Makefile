CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS =


SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
TEST_DIR = tests
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) $(TEST_SRC:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

SERVER = $(BIN_DIR)/server
TEST = $(BIN_DIR)/test

.PHONY: all clean test

all: $(SERVER)

$(BUILD_DIR):
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

$(TEST): $(TEST_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD_DIR)