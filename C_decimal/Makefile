LIB = s21_decimal.a
CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
CFLAGS = -fprofile-arcs -ftest-coverage

OS = $(shell uname)

SRC = s21_decimal.c extra.c
OBJ = s21_decimal.o extra.o

all: test style gcov_report leaks

$(LIB): $(OBJ)
	ar -src $(LIB) $(OBJ)
	ranlib $(LIB)
	rm *.o

$(OBJ): $(SRC)
	$(CC) $(FLAGS) -g -c $(SRC)

main_test:
	gcc -g $(FLAGS) $(SRC) -O2 -lm
	./a.out > analisis.txt

test:
#	@checkmk clean_mode=1 test.check > test.c
	@gcc $(CFLAGS) -g $(FLAGS) $(SRC) test.c -o test `pkg-config --cflags --libs check`
	@./test

gcov_report:
	@mkdir coverage_results
	@lcov --directory . --capture -o coverage_results/lcov.info
	@genhtml -o coverage_results/ coverage_results/lcov.info --highlight --legend --title "Our project coverage"
ifeq ($(OS), Darwin)
	open coverage_results/index.html
else
	xdg-open coverage_results/index.html
endif

clean:
#	rm ./a.out
#	rm test.c
	rm ./*.gc*
	rm test
	rm -rf coverage_results/
	lcov --directory . --zerocounters
	rm ./leaks.txt

style:
	clang-format -n *.c *.h

leaks:
	valgrind --leak-check=full --leak-resolution=low --quiet --log-file=leaks.txt ./test

rebuild: clean test
