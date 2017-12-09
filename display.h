// This display module provides graphics for the battleships game.

struct display;
typedef struct display display;

struct grid;
typedef struct grid grid;

// Create a display object representing a plain white window.
display *newDisplay(char *title);

// Draw a 10x10 grid for the ships
void placeGrid(display *d, grid *g);

// Place a square with a healthy ship
void placeShipSquare(display *d, int startX, int startY, int col, int row);

// Place a square with a hit ship
void placeHitShipSquare(display *d, int startX, int startY, int col, int row);

// Place a square with a sunk ship
void placeSunkShipSquare(display *d, int startX, int startY, int col, int row);

// Place an unknown square
void placeUnknownSquare(display *d, int startX, int startY, int col, int row);

// Place a square with water
void placeWaterSquare(display *d, int startX, int startY, int col, int row);

// Show the next frame and clear the render
void displayFrame(display *d);

// Pause for the given number of milliseconds.
void pause(display *d, int ms);

// Wait for a key press.
char key(display *d);

// Hold the display for a few seconds, then shut down.
void end(display *d);
