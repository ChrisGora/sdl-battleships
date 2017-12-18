.PHONY: battle display show

GCC = gcc -std=c99 -O3 -Wall -pedantic

battle:
	ctags -R
	$(GCC) display.c show.c state.c battle.c -lSDL2 -o battle

display:
	ctags -R
	$(GCC) -DdisplayMain=main display.c -lSDL2 -o display

show:
	ctags -R
	$(GCC) -DshowMain=main show.c display.c -lSDL2 -o show
