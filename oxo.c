/* Play noughts and crosses (tic-tac-toe) between two human players. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

//-----------------------------------------------------------------------------
// Setting up types and constants.  Don't change this section.

// Constants for player X, player O, and neither N.
enum player { X, O, N };
typedef enum player player;

// Constants to represent validity checking.
enum validity { Unchecked, OK, BadFormat, BadLetter, BadDigit, BadCell };
typedef enum validity validity;

// A game object contains the entire current state of a game. It contains the
// grid with each cell recording which player owns it, the player whose turn it
// is next, and the number of moves made in total.
struct game {
    player grid[3][3];
    player next;
    int moves;
};
typedef struct game game;

//-----------------------------------------------------------------------------
// Functions providing the logic of the game.  Develop these.

// Initialize a game, with the given player to move first.
void newGame(game *g, player first) {
	*g = (game) {{{N,N,N},{N,N,N},{N,N,N}}, first, 0};
}

// Convert the letter in a valid move string such as "b2" into a row index.
int row(char *text) {
	int row = text[0] - 'a';
//	printf("text is %c \n", text[0]);
    return row;
}

// Convert the digit in a valid move string such as "b2" into a column index.
int col(char *text) {
	int column = (text[1] - '0') - 1;
//	printf("text is %c \n", text[1]);
    return column;
}

// Check whether a move string typed in by the user such as "b2" is valid, and
// the corresponding cell is available.  Return a validity code.
validity valid(game *g, char *text) {
	if (strlen(text) != 2) return BadFormat;
	if ((text[0] < 'a') || (text[0] > 'c')) return BadLetter;
	if ((text[1] < '1') || (text[1] > '3')) return BadDigit;
	if ((g->grid[row(text)][col(text)]) != N) return BadCell;
    return OK;
}

// Make a move at the given valid position.
void move(game *g, int r, int c) {
//	player *p = g->next;  ASK!!!!!!!!!!!!!!!!!!!!!
	g->grid[r][c] = g->next;
	if (g->next == X) g->next = O;
	else if (g->next == O) g->next = X;
	g->moves = g->moves + 1;
}

// Check if the given line is a winning one, and return the winning player.
player line(player x, player y, player z) {
	if ((x == X) && ((x == y) && (y == z))) return X;
	else if ((x == O) && ((x == y) && (y == z))) return O;
    else return N;
}

bool wonCheck(game *g, int x1, int y1, int x2, int y2, int x3, int y3) {
	bool a = (g->grid[x1][y1] == g->grid[x2][y2]) && (g->grid[x2][y2] == g->grid[x3][y3]);
	bool b = g->grid[x1][y1] != N;
	bool c = g->grid[x2][y2] != N;
	bool d = g->grid[x3][y3] != N;
//	printf("%d %d %d %d %d %dwonCheck says %d \n", x1,y1,x2,y2,x3,y3,c);
	return a && b && c && d;
}

// Check whether a player has won, and return the winning player.
player won(game *g) {
//	player *t = &(g->grid);

	if (wonCheck(g,0,0,0,1,0,2)) return g->grid[0][1];
	if (wonCheck(g,1,0,1,1,1,2)) return g->grid[1][1];
	if (wonCheck(g,2,0,2,1,2,2)) return g->grid[2][1];

	if (wonCheck(g,0,0,1,0,2,0)) return g->grid[1][0];
	if (wonCheck(g,0,1,1,1,2,1)) return g->grid[1][1];
	if (wonCheck(g,0,2,1,2,2,2)) return g->grid[1][2];

	if (wonCheck(g,0,0,1,1,2,2)) return g->grid[1][1];
	if (wonCheck(g,2,0,1,1,0,2)) return g->grid[1][1];
	else return N;
}
/*
player getPlayerFromGrid(game *g, int x, int y)
{
	return g->gid[x][y];
}

player checkLine(int startX, int startY, int stepX, int stepY)
{
}
*/

// Check whether the game has ended in a draw.
bool drawn(game *g) {
	int x = 0;
	while (x <= 2) {
		int y = 0;
		while (y <= 2) {
			if (g->grid[x][y] == N) return false;
			else y++;
		}
		x++;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Playing the game: Don't change this section until after submitting.

// Convert a player constant into a character for printing.
char show(player c) {
    return (c == X) ? 'X' : (c == O) ? 'O' : ' ';
}

// Print a validity error message.
void printInvalid(validity v) {
    if (v == BadFormat) printf("Type a letter and a digit");
    else if (v == BadLetter) printf("Type a letter a, b or c");
    else if (v == BadDigit) printf("Type a digit 1, 2 or 3");
    else if (v == BadCell) printf("Choose an empty cell");
    printf("\n");
}

// Display the grid.
void display(game *g) {
}

// Ask the current player for their move, putting it into the given array.
// Ask repeatedly until the user types in a valid move.
void ask(game *g, char text[100]) {
}

// Play the game interactively between two human players who take turns.
void play(char player) {
}

//-----------------------------------------------------------------------------
// Testing and running: Don't change this section.

// Make sure the constants haven't been changed, so that automarking works.
void checkConstants() {
    assert(X == 0 && O == 1 && N == 2);
    assert(Unchecked == 0 && OK == 1 && BadFormat == 2);
    assert(BadLetter == 3 && BadDigit == 4 && BadCell == 5);
}

// Test initialization of the game (tests 1 to 8)
void testNew(game *g) {
    *g = (struct game) {{{X,X,X},{X,X,X},{X,X,X}},N,-1};
    newGame(g, X);
    assert(g->grid[0][0] == N);
    assert(g->grid[1][1] == N);
    assert(g->grid[2][2] == N);
    assert(g->grid[0][2] == N);
    assert(g->grid[2][1] == N);
    assert(g->next == X);
    assert(g->moves == 0);
    newGame(g, O);
    assert(g->next == O);
}

// Test that the row/col functions give the right answers (tests 9 to 14)
void testRowCol() {
    assert(row("a3") == 0);
    assert(row("b2") == 1);
    assert(row("c2") == 2);
    assert(col("b1") == 0);
    assert(col("b2") == 1);
    assert(col("a3") == 2);
}

// Test invalid input, or trying to play in an occupied cell (tests 15 to 22)
void testInvalid(game *g) {
    newGame(g, X);
    assert(valid(g, "d2") == BadLetter);
    assert(valid(g, "b0") == BadDigit);
    assert(valid(g, "b4") == BadDigit);
    assert(valid(g, "2b") == BadLetter);
    assert(valid(g, "b2x") == BadFormat);
    assert(valid(g, "b") == BadFormat);
    assert(valid(g, "") == BadFormat);
    *g = (game) {{{N,N,N},{N,N,N},{N,X,N}},O,1};
    assert(valid(g, "c2") == BadCell);
}

// Test making a move (tests 23 to 28)
void testMove(game *g) {
    newGame(g, X);
    move(g, row("b2"), col("b2"));
    assert(g->grid[1][1] == X);
    assert(g->next == O);
    assert(g->moves == 1);
    move(g, row("a3"), col("a3"));
    assert(g->grid[0][2] == O);
    assert(g->next == X);
    assert(g->moves == 2);
}

// Test the line function (tests 29 to 36)
void testLine() {
    assert(line(X, X, X) == X);
    assert(line(O, O, O) == O);
    assert(line(X, O, O) == N);
    assert(line(O, X, O) == N);
    assert(line(O, O, X) == N);
    assert(line(N, N, N) == N);
    assert(line(X, N, N) == N);
    assert(line(O, O, N) == N);
}

// Test winning lines (tests 37 to 44)
void testWin(game *g) {
    *g = (game) {{{X,X,X},{N,O,N},{N,O,N}},O,5};
    assert(won(g) == X);
    *g = (game) {{{N,O,N},{X,X,X},{N,O,N}},O,5};
    assert(won(g) == X);
    *g = (game) {{{N,O,N},{N,O,N},{X,X,X}},O,5};
    assert(won(g) == X);
    *g = (game) {{{O,N,N},{O,X,N},{O,N,X}},X,5};
    assert(won(g) == O);
    *g = (game) {{{N,O,N},{X,O,N},{N,O,X}},X,5};
    assert(won(g) == O);
    *g = (game) {{{N,N,O},{X,N,O},{N,N,O}},X,5};
    assert(won(g) == O);
    *g = (game) {{{X,N,O},{N,X,O},{N,N,X}},O,5};
    assert(won(g) == X);
    *g = (game) {{{X,N,O},{N,O,X},{O,N,N}},X,5};
    assert(won(g) == O);
}

// Test no winning line (tests 45 to 48)
void testNoWin(game *g) {
    *g = (game) {{{N,N,N},{N,N,N},{N,N,N}},X,0};
    assert(won(g) == N);
    *g = (game) {{{O,N,X},{X,X,O},{O,X,N}},O,7};
    assert(won(g) == N);
    *g = (game) {{{X,O,X},{X,O,O},{O,X,O}},X,9};
    assert(won(g) == N);
    *g = (game) {{{O,O,X},{X,X,O},{O,X,X}},O,9};
    assert(won(g) == N);
}

// Test drawn games (tests 49 to 50)
void testDraw(game *g) {
    *g = (game) {{{O,N,X},{X,X,O},{O,X,N}},O,7};
    assert(drawn(g) == false);
    *g = (game) {{{O,O,X},{X,X,O},{O,X,X}},O,9};
    assert(drawn(g) == true);
}

// Unit testing.
void test() {
    game *g = malloc(sizeof(game));
    checkConstants();
    testNew(g);
    testRowCol();
    testInvalid(g);
    testMove(g);
    testLine();
    testWin(g);
    testNoWin(g);
    testDraw(g);
    free(g);
    printf("All tests passed.\n");
}

// Run the program with no args to carry out the tests, or with one arg (the
// player to go first) to play the game.
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2 && strcmp(args[1],"X") == 0) play(X);
    else if (n == 2 && strcmp(args[1],"O") == 0) play(O);
    else {
        fprintf(stderr, "Use: ./oxo  OR  ./oxo X  OR  ./oxo O\n");
        exit(1);
    }
    return 0;
}
