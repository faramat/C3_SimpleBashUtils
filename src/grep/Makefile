SHELL := /bin/bash
flags = -Wall -Wextra -Werror
FILE = test.txt

all: s21_grep

s21_grep:
	gcc s21_grep.c -o s21_grep -std=c11

tests:
	bash test_grep.sh

clang:
	clang-format -n *.c
	clang-format -i *.c

clean:
	rm -rf s21_grep

rebuild: clean all