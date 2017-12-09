.PHONY: battle display 

GCC = gcc -std=c99 -O3 -Wall -pedantic

battle:
	$(GCC) battle.c -o battle

display:
	gcc -DdisplayMain=main -std=c99 -pedantic -Wall -O3 display.c -lSDL2 -o display
