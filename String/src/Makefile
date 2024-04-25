CC=gcc
CFLAGS= -std=gnu11 -Isrc
GCOV_FLAGS=--coverage -L. -fprofile-arcs -ftest-coverage 

SRC=src
SOURCES=$(wildcard $(SRC)/*.c)
OBJECTS=$(SOURCES:.c=.o)

all: s21_string.a test

s21_string.a: $(OBJECTS)
	ar -rcs s21_string.a $(OBJECTS)
	ranlib s21_string.a

test: s21_string.a test_string/test.o 
	$(CC) $(CFLAGS) test_string/test.o s21_string.a -o $@

test.o: test_string/test.c  
	$(CC) $(CFLAGS) -c test_string/test.c 

gcov_report: s21_string.a 
	rm -rf *.gcno *.gcov *.gcda *.info report 
	$(CC) $(CFLAGS) $(GCOV_FLAGS) test_string/test.c s21_string.a -o test
	./test
	lcov -t test -o rep.info -c -d .
	genhtml -o report rep.info
	make clean

clean:
	rm -rf *.o $(SRC)/*.o test_string/*.o test
	rm -rf *.gcno *.gcov *.gcda *.info report s21_string.a

style:
	clang-format -style=google -n *.c *.h

rebuild: clean all

.PHONY: clean style rebuild
