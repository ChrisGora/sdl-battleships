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

};

struct grid {
    int x;
    int y;
    int space;

    int squareW;
    int squareH;
    int gridW;
    int gridH;

    char *gridMatrix;
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

static void loadPicture(display *d, char *name, int x, int y, int w, int h) {
    SDL_Surface *s = SDL_LoadBMP(name);
    notNull(s);
    SDL_Texture *t = SDL_CreateTextureFromSurface(d->renderer, s);
    notNull(t);
    //notNeg(SDL_RenderClear(d->renderer));

    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    SDL_RenderCopy(d->renderer, t, NULL, &r);
}

static void colour(display *d, int rgba) {
    int r = (rgba >> 24) & 0xFF;
    int g = (rgba >> 16) & 0xFF;
    int b = (rgba >> 8) & 0xFF;
    int a = rgba & 0xFF;
    notNeg(SDL_SetRenderDrawColor(d->renderer, r, g, b, a));
    //notNeg(SDL_RenderClear(d->renderer));
}

grid *setGrid(int x, int y, char *gridMatrix) {
    grid *g = malloc(sizeof(grid));
    int squareW = 30;
    int squareH = 30;
    int space = 5;
    int gridW = (squareW * 10) + (space * 11);
    int gridH = (squareH * 10) + (space * 11);
    //TODO: Work out why this is giving a billion errors...
    *g = (grid){x, y, 5, squareW, squareH, gridW, gridH, gridMatrix};
    return g;
}


void placeGrid(display *d, grid *g) {
    loadPicture(d, "gridBackground.bmp", g->x, g->y, g->gridW, g->gridH);
    int col = 0;
    int row = 0;
    while (col < 10) {
        
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
    loadPicture(d, "sea.bmp", 0, 0, d->width, d->height);
    loadPicture(d, "gridBackground.bmp", 10, 10, 365, 365);
    loadPicture(d, "testSquare.bmp", 15, 15, 30, 30);
    loadPicture(d, "testSquare.bmp", 50, 15, 30, 30);
    SDL_RenderPresent(d->renderer);
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
}

int displayMain() {
    tests();
    return 0;
}
