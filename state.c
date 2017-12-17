#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "state.h"

static field **newFieldMatrix(int rows, int cols) {
	field **matrix = malloc(rows * sizeof(field *));
	for(int i = 0; i < rows; i++) {
		matrix[i] = malloc(cols * sizeof(field));
	}
	return matrix;
}

static int **newIntMatrix(int rows, int cols) {
	int **matrix = malloc(rows * sizeof(int *));
	for(int i = 0; i < rows; i++) {
		matrix[i] = malloc(cols * sizeof(int));
	}
	return matrix;
}


game *newState() {
    game *g;
	g = malloc(sizeof(struct game));
	g->prim1 = newFieldMatrix(10, 10);
	g->track1 = newFieldMatrix(10, 10);
	g->prim2 = newFieldMatrix(10, 10);
	g->track2 = newFieldMatrix(10, 10);
	g->ships = newIntMatrix(14, 7);

	char copy1[7][20] = {
		{"Carrier"},
		{"Battleship"},
		{"Cruiser"},
		{"Submarine"},
		{"second Submarine"},
		{"Destroyer"},
		{"second Destroyer"}
	};
	int copy2[7] = {5,4,3,3,3,2,2};
	memcpy(g->names, copy1, sizeof(g->names));
	memcpy(g->lengths, copy2, sizeof(g->lengths));
    return g;
}


void emptyAllGrids(game *g) {
	int x = 0;
	while (x <= 9) {
		int y = 0;
		while (y <= 9) {
			g->prim1[x][y] = W;
			g->track1[x][y] = N;
			g->prim2[x][y] = W;
			g->track2[x][y] = N;
			y++;
		}
		x++;
	}
}

void emptyAllShips(game *g) {
	int x = 0;
	while (x <= 13) {
		int y = 0;
		while (y <= 6) {
			g->ships[x][y] = 0;
			y++;
		}
		x++;
	}
}

//------------------------------------------------------------------------------
// GRID / ARRAY selection
int playerGrid(game *g) {
	int a;
	if (g->currentPlayer == 1) a = 0;
	else if (g->currentPlayer == 2) a = 2;
	else a = -1;
	return a;
}

int trackGrid(game *g) {
	int a;
	if (g->currentPlayer == 1) a = 1;
	else if (g->currentPlayer == 2) a = 3;
	else a = -1;
	return a;
}

field **selectArray(int grid, game *g) {
	if (grid == 0) return g->prim1;
	if (grid == 1) return g->track1;
	if (grid == 2) return g->prim2;
	if (grid == 3) return g->track2;
	else return NULL;
}

grid *selectGrid(int grid, game *g) {
	if (grid == 0) return g->prim1D;
	if (grid == 1) return g->track1D;
	if (grid == 2) return g->prim2D;
	if (grid == 3) return g->track2D;
	else return NULL;
}

static bool waterOnlySingle(game *g, int x, int y) {
	if (x > 9) return false;
	if (y > 9) return false;
	if (x < 0) return false;
	if (y < 0) return false;
	if ((g->currentPlayer == 1) && (g->prim1[x][y] != W)) return false;
	if ((g->currentPlayer == 2) && (g->prim2[x][y] != W)) return false;
	return true;
}

bool waterOnlyAll(game *g, int x, int y, int length, char orientation[]) {
	while (length > 0) {
		if (waterOnlySingle(g, x, y) == false) return false;
		if (orientation[0] == 'h') x++;
		else if (orientation[0] == 'v') y++;
		else return false;
		length--;
	}
	return true;
}

static void saveFieldMatrix(FILE *out, field **m, int maxCol, int maxRow) {
    int row = 0;
    int col = 0;
    while (row < maxRow) {
        col = 0;
        while (col < maxCol) {
            int field = m[col][row];
            char f = field + '0';
            fputc(f, out);
            col++;
        }
        row++;
    }
}

static void saveIntMatrix(FILE *out, int **m, int maxCol, int maxRow) {
    int row = 0;
    int col = 0;
    while (row < maxRow) {
        col = 0;
        while (col < maxCol) {
            int field = m[col][row];
            char f = field + '0';
            fputc(f, out);
            col++;
        }
        row++;
    }
}

void saveGame(game *g) {
    FILE *out = fopen("save.battle", "w");
    saveFieldMatrix(out, g->prim1, 10, 10);
    saveFieldMatrix(out, g->track1, 10, 10);
    saveFieldMatrix(out, g->prim2, 10, 10);
    saveFieldMatrix(out, g->track2, 10, 10);
    saveIntMatrix(out, g->ships, 14, 7);

    char f = g->currentPlayer + '0';
    fputc(f, out);

    fclose(out);
}

static void loadFieldMatrix(FILE *in, field **m, int maxCol, int maxRow) {
    int row = 0;
    int col = 0;
    while (row < maxRow) {
        col = 0;
        while (col < maxCol) {
            m[col][row] = fgetc(in) - '0';
            col++;
        }
        row++;
    }
}

static void loadIntMatrix(FILE *in, int **m, int maxCol, int maxRow) {
    int row = 0;
    int col = 0;
    while (row < maxRow) {
        col = 0;
        while (col < maxCol) {
            m[col][row] = fgetc(in) - '0';
            col++;
        }
        row++;
    }
}

void loadGame(game *g) {
    FILE *in = fopen("save.battle", "r");
    loadFieldMatrix(in, g->prim1, 10, 10);
    loadFieldMatrix(in, g->track1, 10, 10);
    loadFieldMatrix(in, g->prim2, 10, 10);
    loadFieldMatrix(in, g->track2, 10, 10);
    loadIntMatrix(in, g->ships, 14, 7);

    g->currentPlayer = fgetc(in) - '0';

    fclose(in);
}
