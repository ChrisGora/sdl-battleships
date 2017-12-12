// This display module provides graphics for the battleships game.

#include <stdbool.h>

struct display;
typedef struct display display;

struct grid;
typedef struct grid grid;

// Create a display object representing a plain white window.
display *newDisplay(char *title, int width, int height);

// Place the background into the buffer
void placeBackground(display *d);

// Create new grid structure with information about a grid of battleships
// Position 1 is left, position 2 is right
grid *newGrid(display *d, int **gridMatrix, int position);

// Draw a 10x10 grid for the ships
void placeGrid(display *d, grid *g, bool selecting);

// Ask the user to select a square and saves the column and row in grid structure
bool setCoords(display *d, grid *g);

// Return the x coordinate selected by the user
int getXcoord(grid *g);

// Return the y coordinate selected by the user
int getYcoord(grid *g);

// Show the next frame and clear the render
void displayFrame(display *d);

// Pause for the given number of milliseconds.
void pause(display *d, int ms);

// Hold the display for a few seconds, then shut down.
void end(display *d);
