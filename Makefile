.PHONY: battle

GCC = gcc -std=c99 -O3 -Wall -Werror -pedantic

battle:
	$(GCC) battle.c -o battle
