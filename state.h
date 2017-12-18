/* This module manages the state of the game, which includes management of all
    field matricies, the ships database and also game saving and loading */

#include <stdbool.h>
#include "display.h"

// Prim is used as the grid with your ships, track is for tracking the enemy ships

// int ships matrix:  14 ships, [0]owner, [1]type of ship, [2]initial length,
// [3]still healthy length, [4]start x loc, [5]start y loc, [6]orient

struct game {
// Current game info
	int currentPlayer;
	field **prim1;
	field **track1;
	field **prim2;
	field **track2;

// Ship names, lengths, positions and health info
	char names[7][20];
	int lengths[7];
	int **ships;

// Previous move info to let the next player know about what happened.
	int x;
	int y;
	field result;
	char input[2];
	bool stored;

// Variables and pointers to help with the display
	display *d;
	bool display;

	grid *prim1D;
	grid *track1D;
	grid *prim2D;
	grid *track2D;
};

typedef struct game game;

game *newState();

void emptyAllGrids(game *g);

void emptyAllShips(game *g);

int playerGrid(game *g);

int trackGrid(game *g);

field **selectArray(int grid, game *g);

grid *selectGrid(int grid, game *g);

bool waterOnlyAll(game *g, int x, int y, int length, char orientation[]);

void saveGame (game *g);

void loadGame (game *g);
