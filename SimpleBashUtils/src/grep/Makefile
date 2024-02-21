CC = gcc
CFLAGS = -std=gnu11 -Wall -Werror -pedantic -Wextra

GREP =  s21_grep.c

GREP_OBJ = s21_grep.o

COVERAGE ?= 0

ifeq ($(COVERAGE), 1)
	CFLAGS += --coverage
endif

all: s21_grep

s21_grep: $(GREP_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(GREP_OBJ): $(GREP)
	$(CC) $(CFLAGS) -c -o $@ $<

run_tests:
	$(shell bash run_tests.sh)

clean:
	rm s21_grep s21_grep.o *.gcda *.gcno
