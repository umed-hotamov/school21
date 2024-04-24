CC = gcc
CFLAGS = -std=gnu11 -Wall -Werror -pedantic -Wextra --coverage

CAT =  s21_cat.c

CAT_OBJ = s21_cat.o

all: s21_cat run_tests clean

s21_cat: $(CAT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(CAT_OBJ): $(CAT)
	$(CC) $(CFLAGS) -c -o $@ $<

run_tests:
	$(shell bash run_tests.sh)

clean:
	rm s21_cat s21_cat.o *.gcda *.gcno
