/* A graphics module for displaying the state of a battleships game */

#include "display.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct display {
    int width;
    int height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *textures[128];
};

struct grid {
    int x;
    int y;
    int space;
    int position;

    int squareW;
    int squareH;
    int gridW;
    int gridH;

    int **gridMatrix;

    int selectedCol;
    int selectedRow;
    bool selected;
    bool confirmed;
};

//------------------------------------------------------------------------------
// SDL ERRORS

// If SDL fails, print the SDL error message, and stop the program.
static void fail() {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
}

// Check return values from SDL functions.
void *notNull(void *p) { if (p == NULL) fail(); return p; }

int notNeg(int n) { if (n < 0) fail(); return n; }

//-----------------------------------------------------------------------------
// PICTURES


//TODO: Handle the letter using typedef instead
static void loadPicture(display *d, char *filename, int letter) {
    SDL_Surface *s = SDL_LoadBMP(filename);
    notNull(s);
    SDL_Texture *t = SDL_CreateTextureFromSurface(d->renderer, s);
    notNull(t);
    d->textures[letter] = t;
    //notNeg(SDL_RenderClear(d->renderer));
}

static void loadAll(display *d) {
    loadPicture(d, "sea.bmp", 'B');
    loadPicture(d, "gridBackground.bmp", 'G');
    loadPicture(d, "testSquare.bmp", 'T');
    loadPicture(d, "healthyShip.bmp", 'S');
    loadPicture(d, "hitShip.bmp", 'X');
    loadPicture(d, "water.bmp", 'W');
    loadPicture(d, "unknown.bmp", 'N');
    loadPicture(d, "sunkShip.bmp", 'U');
    loadPicture(d, "aimTest.bmp", 'A');
}

static void placePicture(display *d, int letter, int x, int y, int w, int h) {
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    SDL_RenderCopy(d->renderer, d->textures[letter], NULL, &r);
}

//------------------------------------------------------------------------------
// BACKGROUND

void placeBackground(display *d) {
    placePicture(d, 'B', 0, 0, d->width, d->height);
}

//------------------------------------------------------------------------------
// GRIDS

static void updateGrid(display *d, grid *g, int **gridMatrix, int position) {
    g->gridMatrix = gridMatrix;
    int screenW = d->width;
    int screenH = d->height;
    int squareW = screenW / 100 * 3;
    int squareH = screenH / 100 * 6;
    if (squareW < squareH) squareH = squareW;
    else squareW = squareH;
    g->squareW = squareW;
    g->squareH = squareH;
    g->space = 5;
    g->gridW = (squareW * 10) + (g->space * 11);
    g->gridH = (squareH * 10) + (g->space * 11);

    g->position = position;
    if (position == 1) {
        g->x = screenW / 4 - g->gridW / 2;
        g->y = screenH / 2 - g->gridH / 2;
    }
    else if (position == 2) {
        g->x = screenW * 3 / 4 - g->gridW / 2;
        g->y = screenH / 2 - g->gridH / 2;
    }
    g->selectedCol = 0;
    g->selectedRow = 0;
}

grid *newGrid(display *d, int **gridMatrix, int position) {
    grid *g = malloc(sizeof(grid));
    updateGrid(d, g, gridMatrix, position);
    g->selected = false;
    g->confirmed = false;
    return g;
}

static void placeOne(display *d, grid *g, int field, int x, int y) {
    char c = -1;
	if (field == 0) c = 'S';
	if (field == 1) c = 'X';
	if (field == 2) c = 'W';
	if (field == 3) c = 'N';
	if (field == 4) c = 'U';
    //printf("char c = %c\n\n", c);
    placePicture(d, c, x, y, g->squareW, g->squareH);
}

void placeGrid(display *d, grid *g) {
    placePicture(d, 'G', g->x, g->y, g->gridW, g->gridH);
    int row = 0;
    int col = 0;
    while (row < 10) {
        col = 0;
        while (col < 10) {
            int field = g->gridMatrix[row][col];
            //printf("field: %d\n", field);
            int x = g->x + g->space + row * (g->squareW + g->space);
            //printf("x %d\n", x);
            int y = g->y + g->space + col * (g->squareH + g->space);
            //printf("y %d\n", y);
            placeOne(d, g, field, x, y);
            if ((g->position == 2) && (g->selectedCol == row) && (g->selectedRow == col)){
                placePicture(d, 'A', x, y, g->squareW, g->squareH);
            }
            col++;
        }
        row++;
    }
}

//------------------------------------------------------------------------------
// DISPLAY

void displayFrame(display *d) {
    SDL_RenderPresent(d->renderer);
    notNeg(SDL_RenderClear(d->renderer));
}

display *newDisplay(char *title, int width, int height) {
    display *d = malloc(sizeof(display));
    d->width = width;
    d->height = height;
    notNeg(SDL_Init(SDL_INIT_VIDEO));
    int x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED;
    d->window = notNull(SDL_CreateWindow(title, x, y, d->width, d->height, SDL_WINDOW_RESIZABLE));
    d->surface = notNull(SDL_GetWindowSurface(d->window));
    d->renderer = notNull(SDL_CreateRenderer(d->window, -1, 0));

    notNeg(SDL_RenderClear(d->renderer));
    //loadPicture(d, "sea.bmp", 0, 0, d->width, d->height);
    //loadPicture(d, "gridBackground.bmp", 10, 10, 365, 365);
    //loadPicture(d, "testSquare.bmp", 15, 15, 30, 30);
    //loadPicture(d, "testSquare.bmp", 50, 15, 30, 30);
    //SDL_RenderPresent(d->renderer);
    loadAll(d);
    return d;
}

void pause(display *d, int ms) {
    if (ms > 0) SDL_Delay(ms);
}

void end(display *d) {
    pause(d, 5000);
    //SDL_FreeSurface(d->surface);
    //SDL_DestroyRenderer(d->renderer);
    //SDL_DestroyWindow(d->window);
    SDL_Quit();
}
//-----------------------------------------------------------------------------
// COORDINATE SELECTION

// 'Selected' shows whether events match the selection
// 'Confirmed' shows whether the player confirmed the target

static void checkCoords(int row, int col, int x, int y, grid *g) {
    int startX = g->x + g->space + row * (g->squareW + g->space);
    int endX = startX + g->squareW;
    //printf("x %d\n", x);
    int startY = g->y + g->space + col * (g->squareH + g->space);
    int endY = startY + g->squareH;
    //printf("y %d\n", y);
    if ((x >= startX) && (x <= endX) && (y >= startY) && (y <= endY)) {
        g->selectedCol = x;
        g->selectedRow = y;
        g->selected = true;
    }
}

static void setCoordsMouse(int x, int y, grid *g) {
    g->selected = false;
    checkCoords(g->selectedRow, g->selectedCol, x, y, g);
    if (g->selected) return;
    int row = 0;
    int col = 0;
    while ((row < 10) && (! g->selected)) {
        col = 0;
        while ((col < 10) && (! g->selected)) {
            checkCoords(row, col, x, y, g);
            col++;
        }
        row++;
    }
}

bool setCoords(display *d, grid *g) {
    g->selected = false;
    g->confirmed = false;
    SDL_Event event_structure;
    SDL_Event *event = &event_structure;
    notNeg(SDL_WaitEvent(event));

    if (event->type == SDL_QUIT) {
        SDL_Quit();
        exit(0);
    }
    else if (event->type == SDL_KEYUP) {
        int sym = event->key.keysym.sym;
        g->selected = true;
        if (sym == SDLK_UP) g->selectedRow--;
        if (sym == SDLK_DOWN) g->selectedRow++;
        if (sym == SDLK_LEFT) g->selectedCol--;
        if (sym == SDLK_RIGHT) g->selectedCol++;
        if (sym == SDLK_SPACE) g->confirmed = true;
    }
    else if (event->type == SDL_MOUSEMOTION) {
        //setCoordsMouse(event->x, event->y, g);
    }
    else if (event->type == SDL_WINDOWEVENT) {

    }
    return g->confirmed;
}
int getXcoord(grid *g) {
    return g->selectedCol;
}

int getYcoord(grid *g) {
    return g->selectedRow;
}


//-----------------------------------------------------------------------------
// TESTING

static void newDisplayTest() {
    display *d = newDisplay("display test", 200, 200);
    end(d);
}

static void loadPictureTest() {
    display *d = newDisplay("picture test", 1000, 500);
    notNeg(SDL_RenderClear(d->renderer));
    loadPicture(d, "sea.bmp", 'B');
    loadPicture(d, "gridBackground.bmp", 'G');
    loadPicture(d, "testSquare.bmp", 'T');
    loadPicture(d, "healthyShip.bmp", 'S');
    loadPicture(d, "hitShip.bmp", 'X');
    loadPicture(d, "water.bmp", 'W');
    loadPicture(d, "unknown.bmp", 'N');
    loadPicture(d, "sunkShip.bmp", 'U');
    placePicture(d, 'B', 0, 0, d->width, d->height);
    placePicture(d, 'G', 10, 10, 365, 365);
    placePicture(d, 'S', 15, 15, 30, 30);
    placePicture(d, 'X', 50, 15, 30, 30);
    placePicture(d, 'W', 15, 50, 30, 30);
    placePicture(d, 'N', 50, 50, 30, 30);
    placePicture(d, 'U', 85, 15, 30, 30);
    displayFrame(d);
    end(d);

    d = newDisplay("picture test - using loadAll", 1000, 500);
    notNeg(SDL_RenderClear(d->renderer));
    loadAll(d);
    placePicture(d, 'B', 0, 0, d->width, d->height);
    placePicture(d, 'G', 10, 10, 365, 365);
    placePicture(d, 'S', 15, 15, 30, 30);
    placePicture(d, 'X', 50, 15, 30, 30);
    placePicture(d, 'W', 15, 50, 30, 30);
    placePicture(d, 'N', 50, 50, 30, 30);
    placePicture(d, 'U', 85, 15, 30, 30);
    displayFrame(d);
    end(d);
}

/*
static void gridTest() {
    display *d = newDisplay("picture test");
    notNeg(SDL_RenderClear(d->renderer));
    placeGrid(d, 10, 10);
    end(d);

    d = newDisplay("picture test");
    notNeg(SDL_RenderClear(d->renderer));
    placeGrid(d, 300, 10);
    end(d);
}
*/

static void tests() {
    newDisplayTest();
    loadPictureTest();
    printf("All tests DONE\n");
}

int displayMain() {
    tests();
    return 0;
}
