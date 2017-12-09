// This display module provides graphics for the battleships game.

struct display;
typedef struct display display;

// Create a display object representing a plain white window.
display *newDisplay(char *title);

// Draw a line from (x0,y0) to (x1,y1) which is black by default.
void line(display *d, int x0, int y0, int x1, int y1);

// Draw a 10x10 grid for the ships
void grid(display *d, int startX, int startY);

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

// Pause for the given number of milliseconds.
void pause(display *d, int ms);

// Clear the display to white.
void clear(display *d);

// Wait for a key press.
char key(display *d);

// Hold the display for a few seconds, then shut down.
void end(display *d);
