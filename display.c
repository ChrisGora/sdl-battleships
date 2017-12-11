/* A graphics module for displaying the state of a battleships game */

#include "display.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct display {
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *textures[128];
};

struct grid {
    int x;
    int y;
    int space;

    int squareW;
    int squareH;
    int gridW;
    int gridH;

    int *gridMatrix;
};

// If SDL fails, print the SDL error message, and stop the program.
static void fail() {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
}

// Check return values from SDL functions.
void *notNull(void *p) { if (p == NULL) fail(); return p; }

int notNeg(int n) { if (n < 0) fail(); return n; }

static void printOne(int a) {
	if (a == 0) printf("S ");
	if (a == 1) printf("X ");
	if (a == 2) printf("# ");
	if (a == 3) printf("- ");
	if (a == 4) printf("* ");
}

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
}

static void placePicture(display *d, int letter, int x, int y, int w, int h) {
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    SDL_RenderCopy(d->renderer, d->textures[letter], NULL, &r);
}


static void colour(display *d, int rgba) {
    int r = (rgba >> 24) & 0xFF;
    int g = (rgba >> 16) & 0xFF;
    int b = (rgba >> 8) & 0xFF;
    int a = rgba & 0xFF;
    notNeg(SDL_SetRenderDrawColor(d->renderer, r, g, b, a));
    //notNeg(SDL_RenderClear(d->renderer));
}

grid *newGrid(int x, int y, int *gridMatrix) {
    grid *g = malloc(sizeof(grid));
    int squareW = 30;
    int squareH = 30;
    int space = 5;
    int gridW = (squareW * 10) + (space * 11);
    int gridH = (squareH * 10) + (space * 11);
    *g = (grid){x, y, 5, squareW, squareH, gridW, gridH, gridMatrix};
    return g;
}


// TODO Design the loop
void placeGrid(display *d, grid *g) {
    //loadPicture(d, "gridBackground.bmp", g->x, g->y, g->gridW, g->gridH);
    int row = 0;
    int col = 0;
    while (row < 10) {
        col = 0;
        while (col < 10) {
            printOne(*g->gridMatrix + col);
            printf("\n");
            col++;
        }
        row++;
    }
}

void displayFrame(display *d) {
    SDL_RenderPresent(d->renderer);
    notNeg(SDL_RenderClear(d->renderer));
}

display *newDisplay(char *title) {
    display *d = malloc(sizeof(display));
    d->width = 1000;
    d->height = 500;
    notNeg(SDL_Init(SDL_INIT_VIDEO));
    int x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED;
    d->window = notNull(SDL_CreateWindow(title, x, y, d->width, d->height, 0));
    d->surface = notNull(SDL_GetWindowSurface(d->window));
    d->renderer = notNull(SDL_CreateRenderer(d->window, -1, 0));

    notNeg(SDL_RenderClear(d->renderer));
    //loadPicture(d, "sea.bmp", 0, 0, d->width, d->height);
    //loadPicture(d, "gridBackground.bmp", 10, 10, 365, 365);
    //loadPicture(d, "testSquare.bmp", 15, 15, 30, 30);
    //loadPicture(d, "testSquare.bmp", 50, 15, 30, 30);
    //SDL_RenderPresent(d->renderer);
    return d;
}

void end(display *d) {
    SDL_Delay(1000);
    //SDL_FreeSurface(d->surface);
    //SDL_DestroyRenderer(d->renderer);
    //SDL_DestroyWindow(d->window);
    SDL_Quit();
}

//-----------------------------------------------------------------------------
// TESTING

static void newDisplayTest() {
    display *d = newDisplay("display test");
    end(d);
}

static void loadPictureTest() {
    display *d = newDisplay("picture test");
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

    d = newDisplay("picture test - using loadAll");
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
