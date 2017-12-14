// This display module provides graphics for the battleships game.

#include <stdbool.h>
#include "field.h"

struct display;
typedef struct display display;

struct grid;
typedef struct grid grid;

// Create a display object representing a plain white window.
display *newDisplay(char *title, int width, int height);

// Place the background into the buffer.
void placeBackground(display *d);

// Create new grid structure with information about a grid of battleships.
// Position 1 is left, position 2 is right
grid *newGrid(display *d, field **gridMatrix, int position);

// Draw a 10x10 grid for the ships.
void placeGrid(display *d, grid *g, bool aiming, bool placing);

// Ask the user to select a square and saves the column and row in grid structure.
bool setCoords(display *d, grid *g);

// Ask the user to select location for a new ship.
bool setShipLocation(display *d, grid *g, int length, char *orientation);

// Return the x coordinate selected by the user (mouse or keyboard).
int getXcoord(grid *g);

// Return the y coordinate selected by the user (mouse or keyboard).
int getYcoord(grid *g);

// Return the ship orientation selected by the user.
char getOrientation(grid *g);

// Automatically update grids and display a new fram
void updateDisplay(display *d, grid *g1, grid *g2, bool aiming, bool placing);

// Show the next frame and clear the render.
void displayFrame(display *d);

// Display a simple message box. The program pauses until OK is pressed or the
// the box is closed.
void displayMessage(display *d, char *title, char* message);

// Pause for the given number of milliseconds.
void pause(display *d, int ms);

// Hold the display for a few seconds, then shut down.
void end(display *d);
