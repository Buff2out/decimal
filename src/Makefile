# Result
NAME			= s21_decimal
TARGET			= $(NAME).a
UNIT_TEST		= $(NAME)_test

# BINs
CC				= gcc
CPPCHECK		= cppcheck
LCOV			= lcov
GENHTML			= genhtml
AR				= ar
RM				= rm
CLANG_FORMAT	= clang-format
VALGRIND		= valgrind

# Flags
CFLAGS			= -Wall -Werror -Wextra -MMD -MP $(addprefix -I, $(INCLUDE_DIRS))
DEBUG_FLAGS		= -g -O0 -DDEBUG
CPPCHECK_FLAGS	= --check-level=exhaustive --enable=all
LIBCHECK_FLAGS	= $(shell pkg-config --cflags --libs check)
COVERAGE_FLAGS	= --coverage
LCOV_FLAGS		= --rc branch_coverage=1 --rc derive_function_end_line=0 --directory .

# Files and Dirs
# Core
SRCS			= $(shell find . -maxdepth 1 -name '*.c')
OBJS			= $(SRCS:.c=.o)
DEPS			= $(OBJS:.o=.d)
# Unit tests
TESTS_DIR		= tests
TESTS			= $(shell find $(TESTS_DIR) -type f -name '*.c')
OBJS_TEST		= $(TESTS:.c=.o)
TESTS_DEPS		= $(OBJS_TEST:.o=.d)
# lcov/gcov
LCOV_INFO		= coverage.info
REPORT_DIR		= report

.PHONY: all clean test gcov_report rebuild debug clang-format


all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rcs $(TARGET) $(OBJS)

$(UNIT_TEST): $(OBJS_TEST)
	$(CC) $(CFLAGS) $(OBJS_TEST) $(OBJS) $(LIBCHECK_FLAGS) -o $(UNIT_TEST)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(OBJS) $(OBJS_TEST) $(TARGET) $(UNIT_TEST) $(LCOV_INFO)
	$(RM) -rf $(REPORT_DIR) $(DEPS) $(TESTS_DEPS) a.out* lib$(TARGET)
	$(RM) -rf $(shell find . -type f -name '*.gcno' -or -name '*.gcda')

test: $(TARGET) $(UNIT_TEST)
	./$(UNIT_TEST)

gcov_report: CFLAGS += $(DEBUG_FLAGS) $(COVERAGE_FLAGS)
gcov_report: clean test
	$(LCOV) --zerocounters $(LCOV_FLAGS)
	$(LCOV) --capture --initial $(LCOV_FLAGS) --output-file $(LCOV_INFO)
	./$(UNIT_TEST)
	$(LCOV) --no-checksum --capture $(LCOV_FLAGS) --output-file $(LCOV_INFO)
	$(GENHTML) --branch-coverage --ignore-errors unsupported --legend --output-directory $(REPORT_DIR) $(LCOV_INFO)
	open ./$(REPORT_DIR)/index.html

rebuild: clean all

-include $(DEPS) $(TESTS_DEPS)

debug:
	$(CC) $(DEBUG_FLAGS) $(SRCS) main.c
	./a.out


clang-format:
	$(CLANG_FORMAT) -i $(shell find . -type f -name '*.h' -or -name '*.c')
