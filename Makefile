CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c11
TEST_FLAGS = -lcheck -lm -lpthread

SRC_DIR = src
UTILS_DIR = $(SRC_DIR)/utils
TEST_DIR = test

UTILS_SRC = $(UTILS_DIR)/utils.c
UTILS_OBJ = $(UTILS_SRC:.c=.o)

TEST_SRC = $(TEST_DIR)/test_utils.c
TEST_EXEC = $(TEST_DIR)/test_utils

.PHONY: all clean test

all: utils

utils: $(UTILS_OBJ)

test: $(TEST_EXEC)
	$(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC) $(UTILS_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(UTILS_OBJ) $(TEST_EXEC)