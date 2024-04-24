CC = gcc
FLAGS = -std=c11 -Wall -Wextra -Werror -g 
GCOVFLAGS = -L. -ls21_string --coverage 
OS = $(shell uname)
ifeq ($(OS), Linux)
	DEF_FLAG = -D LINUX
	CHECK_FLAGS = -lcheck -pthread -lrt -lm -lsubunit 
else
	DEF_FLAG = -D MACOS
	CHECK_FLAGS = $(shell pkg-config --cflags check) $(shell pkg-config --libs check)
endif

FORMAT = $(shell find . -type f \( -name "*.h" -o -name "*.c" \) -exec clang-format -i {} \;)

TEST_DIR = tests
TEST_SRCS = s21_test.c
TEST_HEADER = s21_test.h
TEST_HEADER_PATH = $(addprefix $(TEST_DIR)/, $(TEST_HEADER))
# test base
TEST_BASE_DIR = base
TEST_CREATED_DIRS = $(TEST_BASE_DIR)
TEST_BASE = s21_memchr_test.c \
	s21_memcmp_test.c \
	s21_memcpy_test.c \
	s21_memset_test.c \
	s21_strchr_test.c \
	s21_strcspn_test.c \
	s21_strerror_test.c \
	s21_strlen_test.c \
	s21_strncat_test.c \
	s21_strncmp_test.c \
	s21_strncpy_test.c \
	s21_strpbrk_test.c \
	s21_strrchr_test.c \
	s21_strsrt_test.c \
	s21_strtok_test.c
TEST_BASE_PATH = $(addprefix $(TEST_BASE_DIR)/, $(TEST_BASE))
TEST_SRCS += $(TEST_BASE_PATH)
# test bonus
TEST_BONUS_DIR = bonus
TEST_CREATED_DIRS += $(TEST_BONUS_DIR)
TEST_BONUS = s21_insert_test.c \
	s21_to_lower_test.c \
	s21_to_upper_test.c \
	s21_trim_test.c
TEST_BONUS_PATH = $(addprefix $(TEST_BONUS_DIR)/, $(TEST_BONUS))
TEST_SRCS += $(TEST_BONUS_PATH)
# test sprintf
TEST_SPRINTF_DIR = sprintf
TEST_CREATED_DIRS += $(TEST_SPRINTF_DIR)
TEST_SPRINTF = s21_sprintf_c_test.c \
	s21_sprintf_d_test.c \
	s21_sprintf_e_test.c \
	s21_sprintf_f_test.c \
	s21_sprintf_g_test.c \
	s21_sprintf_i_test.c \
	s21_sprintf_n_test.c \
	s21_sprintf_ouxX_test.c \
	s21_sprintf_s_test.c
TEST_SPRINTF_PATH = $(addprefix $(TEST_SPRINTF_DIR)/, $(TEST_SPRINTF))
TEST_SRCS += $(TEST_SPRINTF_PATH)

TEST_SRCS_PATH = $(addprefix $(TEST_DIR)/, $(TEST_SRCS))
# test objs
TEST_OBJS_DIR = test_objs
TEST_OBJS = $(patsubst %.c, %.o, $(TEST_SRCS))
TEST_OBJS_PATH = $(addprefix $(TEST_OBJS_DIR)/, $(TEST_OBJS))
TEST_OBJS_CREATES_DIR = $(addprefix $(TEST_OBJS_DIR)/, $(TEST_CREATED_DIRS))


# TEST_COMPILE = $(CC) $(TEST_FLAGS) $(TEST_MAIN) $(TEST_FILES) s21_string.a $(CHECK_FLAGS) -L. -ls21_string -I tests

all: $(FORMAT) s21_string.a

rebuild: $(FORMAT) clean all

$(TEST_OBJS_DIR)/%.o: $(TEST_DIR)/%.c $(TEST_HEADER_PATH) Makefile
	mkdir -p $(TEST_OBJS_DIR) $(TEST_OBJS_CREATES_DIR)
	$(CC) $(FLAGS) $(CHECK_FLAGS) -I $(TEST_DIR) -Qunused-arguments -c $< -o $@

test: s21_string.a $(TEST_OBJS_PATH)
	$(FORMAT)
	$(CC) $(FLAGS)  $(CHECK_FLAGS) $(TEST_OBJS_PATH) s21_string.a -o s21_test
	./s21_test

gcov_report: s21_string.a test
	$(FORMAT)
	rm -f *.g*
	$(CC) $(TEST_FLAGS) $(DEF_FLAG) functions/s21*.c ./s21_test.o tests/*/*.c libs21_string.a $(CHECK_FLAGS) $(GCOVFLAGS)
	./a.out
	lcov -t a.out -o rep.info -c -d .
	genhtml -o report rep.info
	open ./report/index.html
	rm -rf *.gcda *.gcno s21_tests s21_tests.info

s21_string.a: s21_string.o
	$(FORMAT)
	ar rcs libs21_string.a *.o
	rm -rf *.o
	ranlib libs21_string.a
	cp libs21_string.a s21_string.a
	cp libs21_string.a ls21_string.a

s21_string.o:
	$(FORMAT)
	$(CC) $(FLAGS) -c functions/s21*.c

s21_test.o:
	$(FORMAT)
	$(CC) $(FLAGS) -c $(TEST_FILES) $(TEST_MAIN)

check: all
	$(FORMAT)
	cppcheck --enable=all --suppress=missingIncludeSystem *.h functions/*.c $(TEST_FILES) $(TEST_MAIN)
	clang-format -n *.h functions/*.c $(TEST_FILES)
ifeq ($(OS), Linux)
	$(CC) $(TEST_FLAGS) $(TEST_FILES) libs21_string.a $(CHECK_FLAGS) -D STRANGE -L. -ls21_string
	touch valgrind_res.txt
	valgrind --leak-check=full --log-file="valgrind_res.txt" --track-origins=yes --show-reachable=yes ./a.out
	grep "ERROR" valgrind_res.txt
endif

clean:
	rm -rf test_objs
	rm -rf *.o *.g* *.info *.out report *.a *.dSYM debug valgrind_res.txt tests/*.log tests/*.xml
