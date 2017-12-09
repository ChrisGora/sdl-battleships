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

// If SDL fails, print the SDL error message, and stop the program.
static void fail() {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
}

// Check return values from SDL functions.
void *notNull(void *p) { if (p == NULL) fail(); return p; }
int notNeg(int n) { if (n < 0) fail(); return n; }

void loadBackground(display *d) {
    SDL_Surface *background = SDL_LoadBMP("sea.bmp");
    notNull(background);
    notNeg(SDL_BlitSurface(background, NULL, d->surface, NULL));
}

display *newDisplay(char *title) {
    display *d = malloc(sizeof(display));
    d->width = 1000;
    d->height = 500;
    notNeg(SDL_Init(SDL_INIT_VIDEO));
    int x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED;
    d->window = notNull(SDL_CreateWindow(title, x, y, d->width, d->height, 0));
    d->surface = notNull(SDL_GetWindowSurface(d->window));
    d->renderer = notNull(SDL_CreateSoftwareRenderer(d->surface));
    loadBackground(d);
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
    SDL_RenderClear(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    return d;
}

void end(display *d) {
    SDL_Delay(5000);
    SDL_Quit();
}

//-----------------------------------------------------------------------------
// TESTING

static void newDisplayTest() {
    display *d = newDisplay("Battleships");
    end(d);
}

static void tests() {
    newDisplayTest();
}

int displayMain() {
    tests();
    return 0;
}
