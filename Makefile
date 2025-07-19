CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c11
TEST_FLAGS = -lcheck -lm -lpthread

SRC = src/utils/utils.c src/operations.c
OBJ = $(SRC:.c=.o)

TEST_SRC = test/test.c test/test_utils.c test/test_add.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXEC = test/test

.PHONY: all clean test

all: utils

utils: $(OBJ)

test: $(TEST_EXEC)
	$(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC) $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_EXEC)