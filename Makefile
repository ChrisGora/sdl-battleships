.PHONY: battle display

GCC = gcc -std=c99 -O3 -Wall -pedantic

battle:
	ctags -R
	$(GCC) battle.c display.c -lSDL2 -o battle

display:
	ctags -R
	gcc -DdisplayMain=main -std=c99 -pedantic -Wall -O3 display.c -lSDL2 -o display
