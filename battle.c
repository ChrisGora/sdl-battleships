/* Battleships!!! */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// S is a ship that hasn't been hit
// X is a ship that has been hit
// W is known water - printed as '-'
// N is unknown
// U is a completely sunk ship

enum field {S, X, W, N, U};
typedef enum field field;

//TODO: Use the validity enums or get rid of them

enum validity { Unchecked, OK, BadFormat, BadLetter, BadDigit, BadCell };
typedef enum validity validity;

// Prim is used as the grid with your ships, track is for tracking the enemy ships

// int ships matrix:  14 ships, [0]owner, [1]type of ship, [2]initial length, 
// [3]still healthy length, [4]start x loc, [5]start y loc, [6]orient

struct game {
	int currentPlayer;
	char prim1[10][10];
	char track1[10][10];
	char prim2[10][10];
	char track2[10][10];
	char names[7][20];
	int lengths[7];
	int ships[14][7];
    //INFO about the previous move to let the next player know about what happened.    
	int x;
	int y;
	field result;
	char input[2];
	bool stored;
	
};
typedef struct game game;

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

void clear() {
	int i = 0;
	while (i < 200) {
		printf("\n");
		i++;
	}
}

// prim1 is 0
// track1 is 1
// prim2 is 2
// track2 is 3

// Return an int corresponding to the primary grid
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

void printOne(int a) {
	if (a == 0) printf("S ");
	if (a == 1) printf("X ");
	if (a == 2) printf("# ");
	if (a == 3) printf("- ");
	if (a == 4) printf("* ");
}

void printKey() {
	printOne(0);
	printf("is a healthy ship square\n");
	printOne(1);
	printf("is a hit ship square\n");
	printOne(2);
	printf("is a known water square\n");
	printOne(3);
	printf("is an unknown square\n");
	printOne(4);
	printf("is a completely sunken ship\n\n");	
}

void printGrid(int grid, game *g) {
	printf("\n");
	int y = 0;
	char col = 'a';
	printf("  ");	
	while (col <= 'j') {
		printf("%c ", col);
		col++;
	}
	printf("\n");
	while (y <= 9) {
		int x = 0;
		printf("%d ", y);
		while (x <= 9) {
			if (grid == 0) printOne(g->prim1[x][y]);
			if (grid == 1) printOne(g->track1[x][y]);
			if (grid == 2) printOne(g->prim2[x][y]);
			if (grid == 3) printOne(g->track2[x][y]);
			x++;
		}
		printf("\n");
		y++;
	}
	printf("\n \n");
}

void newGame(game *g) {
	clear();
	printf("--- BATTLESHIPS --- \n");
	printf("Starting a new game \n \n \n");
	emptyAllGrids(g);
	emptyAllShips(g);
	g->currentPlayer = 1;
//	printGrid(playerGrid(g), g);
}

//Convert a letter to column index
int col(char *text) {
	int col = text[0] - 'a';
//	printf("text is %c \n", text[0]);
    return col;
}

//Convert a letter to a row index
int row(char *text) {
	int row = (text[1] - '0');
//	printf("text is %c \n", text[1]);
    return row;
}

bool waterOnlySingle(game *g, int x, int y) {
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

//Place a single ship
void placeShip(game *g, int x, int y, int length, char orientation[]) {
	while (length > 0) {
		if (g->currentPlayer == 1) {
			g->prim1[x][y] = S;
//			g->p1Ships[] = g->p1Ships + 1;
			printOne(g->prim1[x][y]);
			printf("\n");
		}
		else if (g->currentPlayer == 2) {
			g->prim2[x][y] = S;
			printOne(g->prim2[x][y]);
			printf("\n");
		}
		if (orientation[0] == 'h') x++;
		else if (orientation[0] == 'v') y++;
		length--;
	}
}

void sinkShip(game *g, int id) {
	int owner = g->ships[id][0];
	int x = g->ships[id][4];
	int y = g->ships[id][5];
	char orientation = g->ships[id][6];
	int length = g->ships[id][2];
	while (length > 0) {
		if (owner == 2) {
			g->prim2[x][y] = U;
			g->track1[x][y] = U;
//			g->p1Ships[] = g->p1Ships + 1;
//			printOne(g->prim1[x][y]);
//			printf("\n");
		}
		else if (owner == 1) {
			g->prim1[x][y] = U;
			g->track2[x][y] = U;
//			printOne(g->prim2[x][y]);
//			printf("\n");
		}
		if (orientation == 'h') x++;
		else if (orientation == 'v') y++;
		length--;
	}
}

//IO: Ask where to place the ship and which way round

void askWhere(char ship[], int length, char location[], char orientation[]) {
	printf("\n\n---INSTRUCTIONS---\n");
	printf("Place a %s which is %d squares long \nGive two coordinates and press [Enter]\n\n", ship, length);
	printf("Type the coordinates in the form (letter)(number) - for example a5\n");
	printf("Confirm with [Enter] - This will select a starting point for the ship\n");
	printf("Then specify if the ship is meant to be horizontal or vertical\n");
	printf("Based on the length of the ship, appropriate squares will be filled\n");
	printf("EXAMPLE: When placing a Submarine (3 squares long)\n");
	printf("typing in 'a5' followed by 'h' would fill in a5, a6 and a7\n\n");
	fgets(location, 20, stdin);
//	printf("col: %c , row: %c \n", location[0], location[1]);
	printf("Horizontal or vertical? (please type h or v) \n \n");
	fgets(orientation, 20, stdin);
}

void registerShip(game *g, int x, int y, int id, int shipLength, char orientation[]) {
//	int shipID = 0;
	int shipID = id;
	printf("ship ID %d\n", shipID);
	printf("curr player %d", g->currentPlayer);
//	if (g->currentPlayer == 1) shipID = id - 1;
//	else if (g->currentPlayer == 2) shipID = id - 1 + 7;
	g->ships[shipID][0] = g->currentPlayer;
	g->ships[shipID][1] = shipID;
	g->ships[shipID][2] = shipLength;
	g->ships[shipID][3] = shipLength;
	g->ships[shipID][4] = x;
	g->ships[shipID][5] = y;
	g->ships[shipID][6] = orientation[0];
}

void locationSelect(game *g, int id, char ship[], int shipLength) {
	clear();
	printKey();
	printGrid(playerGrid(g), g);	
	char loc[5];
	char orient[5];
	orient[1] = 'f';
	int x = -1;
	int y = -1;
	bool valid = false;
	while (valid == false) {
		askWhere(ship, shipLength, loc, orient);
//		printf("col: %c , row: %c \n", loc[0], loc[1]);
		x = col(loc);
		y = row(loc);
		valid = waterOnlyAll(g, x, y, shipLength, orient);
		if (valid == false) printf("INCORRECT, please try again \n");
	}
	placeShip(g, x, y, shipLength, orient);
	registerShip(g, x, y, id, shipLength, orient);
	printGrid(playerGrid(g), g);
}

//Ask players to place all of their ships
void fillNewIn(game *g) {
	int i = 0;
	int n = -10;
	if (g->currentPlayer == 1) n = 0;
	else if (g->currentPlayer == 2) n = 7;
	while (i < 7) {
	    locationSelect(g, n, g->names[i], g->lengths[i]);
		printf("fillNewIn i %d n %d", i, n);
        i++;
		n++;
	}
}

void nextPlayer(game *g) {
	if (g->currentPlayer == 1) g->currentPlayer = 2;
	else if (g->currentPlayer == 2) g->currentPlayer = 1;
}

//find the starting x y coords 
// 

int minShipId(game *g, bool ownShip) {
	int id;
	if ((g->currentPlayer == 1) && ownShip) id = 0;
	if ((g->currentPlayer == 2) && ownShip) id = 7;
	if ((g->currentPlayer == 1) && (! ownShip)) id = 7;
	if ((g->currentPlayer == 2) && (! ownShip)) id = 0;
	return id;
}

int maxShipId(game *g, bool ownShip) {
	int id;
	if ((g->currentPlayer == 1) && ownShip) id = 6;
	if ((g->currentPlayer == 2) && ownShip) id = 13;
	if ((g->currentPlayer == 1) && (! ownShip)) id = 13;
	if ((g->currentPlayer == 2) && (! ownShip)) id = 6;
	return id;
}

//NOTE: You should only attempt to locate a ship. Don't try to locate water
//Tested pretty well - no bugs

int locateShip(game *g, int x, int y, bool ownShip) {
	int id = minShipId(g, ownShip);
	int idMax = maxShipId(g, ownShip);
	bool located = false;
	while ((! located) && (id <= idMax)) {
		int checkX = x;
		int checkY = y;
		while ((! located) && (checkX >= 0) && (checkY >= 0)) {
			printf("checkX %d , checkY %d, id %d\n", checkX, checkY, id);
			if ((g->ships[id][1]) == 0) id++;
			else if ((g->ships[id][4] == checkX) && (g->ships[id][5] == checkY)) located = true;
			else if ((g->ships[id][6]) == ('v')) checkY = checkY - 1;
			else if ((g->ships[id][6]) == ('h')) checkX = checkX - 1;
			printf("located %d \n", located);
		}
		if (! located) id++;
	}
	assert((id >= 0) && (id <= 13));
	if (located) return id;
	else return -1;
}

// update the ship health data if a ship has been shot

void updateHitShipData(game *g, int x, int y) {
	int id = locateShip(g, x, y, false);
	g->ships[id][3] = g->ships[id][3] - 1;
}
	

field shoot(game *g, int x, int y) {
	int cell = N;
	if ((g->currentPlayer == 1) && (g->prim2[x][y] == S)) cell = X;
	else if ((g->currentPlayer == 2) && (g->prim1[x][y] == S)) cell = X;
	else cell = W;
	if (g->currentPlayer == 1) g->track1[x][y] = cell;
	if (g->currentPlayer == 2) g->track2[x][y] = cell;
	if (g->currentPlayer == 1) g->prim2[x][y] = cell;
	if (g->currentPlayer == 2) g->prim1[x][y] = cell;	
	return cell;
}

//void aim(game *g)

field turn(game *g, int x, int y) {
	field cell = shoot(g, x, y);
	if (cell == X) {
		updateHitShipData(g, x, y);
	}
	return cell;
}

int healthSumPlayer1(game *g) {
	int i = 0;
	int sum = 0;
	while (i < 7) {
		sum = sum + g->ships[i][3];
		i++;
	}
	return sum;
}

int healthSumPlayer2(game *g) {
	int i = 7;
	int sum = 0;
	while (i < 14) {
		sum = sum + g->ships[i][3];
		i++;
	}
	return sum;
}

void won(game *g) {
	printf("Congratulation! Player %d won!!! \n \n", g->currentPlayer);
	printf("Start a new game? \n");
}

void whoNext(game *g) {
	printf("Up Next: Player %d \n", g->currentPlayer);
}

void confirmContinue() {
	printf("Please press [Enter] to continue\n");
	char ch;
	while (ch != '\r' && ch != '\n') { 
		ch = getchar();
	}
}

bool validTarget(game *g, int x, int y) {
	if ((x > 9) || (x < 0)) return false;
	if ((y > 9) || (y < 0)) return false;
	if ((g->currentPlayer == 1) && (g->track1[x][y] != N)) return false;
	if ((g->currentPlayer == 2) && (g->track2[x][y] != N)) return false;	
	else return true;
}

void selectTarget(char location[]) {
	printf("\nCALL YOUR SHOT\nPlease select your next target \n \n");
	fgets(location, 20, stdin);
//	printf("col: %c , row: %c \n", location[0], location[1]);
}

//Translate ID in range 0-13 to ID in range 0-6
int longToShortID(int longID) {
	int shortID = -1;
	if ((longID >= 0) && (longID <= 6)) shortID = longID;
	if ((longID >= 7) && (longID <= 13)) shortID = longID - 7;
	return shortID;
}

//TODO: let them know which ship it was

bool sunk(game *g, int id) {
	bool isSunk;
	if (g->ships[id][3] == 0) isSunk = true;
	else isSunk = false;
	if (isSunk == true) {
		sinkShip(g, id);
	}
	printf("sunk says %d", isSunk);
	return isSunk;
}

void letKnow(game *g, int x, int y, field result, char input[]) {
	printf("\n\nYou shot at %c%c and ", input[0], input[1]);
	if (result == W) printf("you MISSED\n\n");
	if (result == X) {
		printf("you HIT a ship!\n");
		int id = locateShip(g, x, y, false);
		bool isSunk = sunk(g, id);
		if (isSunk == true) {
			id = longToShortID(id);
			printf("It was a %s and it has been SUNK \n\n", g->names[id]);
		}
		else printf("\n");
	}
}

void letOpponentKnow(game *g) {
	if (g->stored) {
		printf("\n\nYour opponent shot at %c%c and ", g->input[0], g->input[1]);
		if (g->result == W) printf("they MISSED\n");
		if (g->result == X) {
			printf("they HIT a ");
			int id = locateShip(g, g->x, g->y, true);
			bool isSunk = sunk(g, id);
			id = longToShortID(id);
			printf("%s", g->names[id]);
			if (isSunk == true) printf(" and it has been SUNK\n");
			else printf("\n");
		}
	}
}

void store(game *g, int x, int y, field result, char input[]) {
	g->x = x;
	g->y = y;
	g->result = result;
	memcpy(g->input, input, sizeof(g->input));
	g->stored = true;
}

void printPlayersGrids(game *g) {
	printf("YOUR OWN SHIPS: \n");
	printGrid(playerGrid(g), g);
	printf("WHAT YOU KNOW ABOUT ENEMY SHIPS: \n");
	printGrid(trackGrid(g), g);

}

void printHealth(game *g) {
	int p1 = (healthSumPlayer1(g)*100)/22;
	int p2 = (healthSumPlayer2(g)*100)/22;
	printf("Player 1 Health: %d%% \n", p1);
	printf("Player 2 Health: %d%% \n\n", p2);
	int shortID = 0;
	int longID = minShipId(g, true);
	int maxLongID = maxShipId(g, true);
	while (longID <= maxLongID) {
		int healthInit = g->ships[longID][2];
		int healthNow = g->ships[longID][3];
		int healthPercent = healthNow * 100 / healthInit;
		printf("%-20s%-1d/%-1d (%-3d%%) \n", g->names[shortID], healthNow, healthInit, healthPercent);
		shortID++;
		longID++;
	}
}

void target(game *g) {
	char position[20];
	int x = 0;
	int y = 0;
	bool valid = false;
	while (! valid) {
		selectTarget(position);
		x = col(position);
		y = row(position);
		valid = validTarget(g, x, y);
		if (! valid) printf("Please try again \n");
	}
	field result = turn(g, col(position), row(position));
	clear();
    printPlayersGrids(g);
	letKnow(g, col(position), row(position), result, position);
	store(g, col(position), row(position), result, position);
}

//TODO: Label sunken ships using asterisks!!! design a sunk function
//TODO: Include a key to the grids
//TODO: Maybe singleplayer???

void playGame(game *g) {
	newGame(g);
	whoNext(g);
	confirmContinue();
	fillNewIn(g);
	clear();
	nextPlayer(g);
	whoNext(g);
	confirmContinue();
	fillNewIn(g);
	while ((healthSumPlayer1(g) > 0) && (healthSumPlayer2(g) > 0)) {
		clear();
		nextPlayer(g);
		whoNext(g);
		confirmContinue();
        clear();
		printKey();
		printPlayersGrids(g);
		printHealth(g);
		letOpponentKnow(g);
		target(g);
		confirmContinue();
	}
	won(g);
	confirmContinue();
}

void emptyGridsTest(game *g) {
	emptyAllGrids(g);
	assert(g->prim1[0][0] == W);
	assert(g->prim2[0][0] == W);
	assert(g->track1[0][0] == N);
	assert(g->track2[0][0] == N);

	assert(g->prim1[9][0] == W);
	assert(g->prim2[9][0] == W);
	assert(g->track1[9][0] == N);
	assert(g->track2[9][0] == N);

	assert(g->prim1[0][9] == W);
	assert(g->prim2[0][9] == W);
	assert(g->track1[0][9] == N);
	assert(g->track2[0][9] == N);

	assert(g->prim1[9][9] == W);
	assert(g->prim2[9][9] == W);
	assert(g->track1[9][9] == N);
	assert(g->track2[9][9] == N);
}

void placingShipsTestPlayer1(game *g) {
	emptyAllGrids(g);
	g->currentPlayer = 1;
	placeShip(g,0,0,5,"h");
	assert(g->prim1[0][0] == S);
	assert(g->prim1[4][0] == S);
}

void placingShipsTestPlayer2(game *g) {
	emptyAllGrids(g);
	g->currentPlayer = 2;
	placeShip(g,0,0,10,"v");
	assert(g->prim2[0][0] == S);
	assert(g->prim2[0][9] == S);
}
	
void emptyShipsTest(game *g) {
	emptyAllShips(g);
	assert(g->ships[0][0] == 0);
	assert(g->ships[13][6] == 0);
}

void shipRegTest(game *g) {
	emptyAllGrids(g);
	g->currentPlayer = 1;
	placeShip(g, 0, 0, 5, "h");
	registerShip(g, 0, 0, 2, 5, "h");
	printGrid(playerGrid(g), g);
	assert(g->ships[2][0] == 1);
	assert(g->ships[2][1] == 2);
	assert(g->ships[2][2] == 5);
	assert(g->ships[2][3] == 5);
	assert(g->ships[2][4] == 0);
	assert(g->ships[2][5] == 0);
	assert(g->ships[2][6] == 'h');
	
	emptyAllGrids(g);
	g->currentPlayer = 2;
	placeShip(g, 3, 2, 7, "v");
	registerShip(g, 3, 2, 8, 7, "v");
	printGrid(playerGrid(g), g);
	assert(g->ships[8][0] == 2);
	assert(g->ships[8][1] == 8);
	assert(g->ships[8][2] == 7);
	assert(g->ships[8][3] == 7);
	assert(g->ships[8][4] == 3);
	assert(g->ships[8][5] == 2);
	assert(g->ships[8][6] == 'v');
}

void locationTest(game *g) {
	assert(locateShip(g, 3, 7, true) == 8);
}

void shootTest(game *g) {
	g->currentPlayer = 1;
	printGrid(playerGrid(g), g);
	printGrid(trackGrid(g), g);
	assert(g->prim2[3][7] == S);
	int before = g->ships[8][3];
	turn(g, 3, 7);
	int after = g->ships[8][3];
	printGrid(playerGrid(g), g);
	printGrid(trackGrid(g), g);
	assert(g->prim2[3][7] == X);
	assert(g->track1[3][7] == X);
	assert(after == before - 1);
	letKnow(g, 3, 7, X, "d7");
}

void weirdBugTest(game *g) {
	emptyAllGrids(g);
	printGrid(playerGrid(g), g);
	printGrid(trackGrid(g), g);
	g->currentPlayer = 2;
	placeShip(g, 0, 3, 5, "v");
	registerShip(g, 0, 3, 12, 5, "v");
	printGrid(playerGrid(g), g);
	printGrid(trackGrid(g), g);
	assert(locateShip(g, 0, 5, true) == 12);

	emptyAllGrids(g);
	emptyAllShips(g);
	g->currentPlayer = 1;
	placeShip(g, 7, 9, 3, "h");
	registerShip(g, 7, 9, 4, 3, "h");
	printGrid(playerGrid(g), g);
	printGrid(trackGrid(g), g);
	g->currentPlayer = 2;
	assert(locateShip(g, 9, 9, false) == 4);
	assert(locateShip(g, 8, 9, false) == 4);
	assert(locateShip(g, 7, 9, false) == 4);
	turn(g, 9, 9);
	turn(g, 8, 9);
	turn(g, 7, 9);
	sunk(g, (locateShip(g, 7, 9, false)));
	printGrid(playerGrid(g), g);
	printGrid(trackGrid(g), g);
	assert(g->prim1[8][9] == U);

}

void sinkingTest(game *g) {
	emptyAllGrids(g);
	g->currentPlayer = 2;
	placeShip(g, 0, 0, 10, "v");
	registerShip(g, 0, 0, 7, 10, "v");
	g->currentPlayer = 1;
	sinkShip(g, 7);
	assert(g->prim2[0][0] == U);
	assert(g->prim2[0][9] == U);
	assert(g->track1[0][0] == U);
	assert(g->track1[0][9] == U);
	printGrid(playerGrid(g), g);
	printGrid(trackGrid(g), g);
}

void tests(game *g) {
	emptyGridsTest(g);
	placingShipsTestPlayer1(g);
	placingShipsTestPlayer2(g);
	newGame(g);
	emptyShipsTest(g);
	shipRegTest(g);
	locationTest(g);
	shootTest(g);
	weirdBugTest(g);
	sinkingTest(g);
	printf("tests passed!\n");	
}

int main(int n, char *args[n]) {
    for (int i=0; i<n; i++) {
//		printf("Arg %d is %s\n", i, args[i]);
	}
	game *g;
	g = malloc(sizeof(struct game));
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
	if (n > 1) tests(g);
	else while (true) playGame(g);
	return 0;
}
