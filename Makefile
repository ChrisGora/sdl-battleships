.PHONY: hello grade grade2 calendar triangle string bits moo oxo battle

GCC = gcc -std=c99 -O3 -Wall -Werror -pedantic

hello:
	$(GCC) hello.c -o hello
	./hello

square:
	$(GCC) square.c -o square
	./square

grade:
	$(GCC) grade.c -o grade

grade2:
	$(GCC) grade2.c -o grade2

calendar:
	$(GCC) calendar.c -o calendar

triangle:
	$(GCC) triangle.c -o triangle

string:
	$(GCC) string.c -o string

bits:
	$(GCC) bits.c -o bits

moo:
	$(GCC) moo.c -o moo

oxo:
	$(GCC) oxo.c -o oxo

battle:
	$(GCC) battle.c -o battle
