SHELL := /bin/bash
flags = -Wall -Wextra -Werror
FILE = test.txt

all: s21_cat

s21_cat:
	gcc $(flags) s21_cat.c -o s21_cat -std=c11

tests:
	bash test_cat.sh

clang:
	clang-format -n *.c
	clang-format -i *.c

clean:
	rm -rf s21_cat

rebuild: clean all