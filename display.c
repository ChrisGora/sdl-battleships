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

    int prevWidth;
    int prevHeight;

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

    field **gridMatrix;

    int selectedCol;
    int selectedRow;
    bool selected;
    bool confirmed;

    bool save;

// Only used when placing new ships
    char orientation;
    int length;
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
    //SDL_Surface *optimisedS = SDL_ConvertSurface(s, )
    SDL_Texture *t = SDL_CreateTextureFromSurface(d->renderer, s);
    notNull(t);
    d->textures[letter] = t;
    //notNeg(SDL_RenderClear(d->renderer));
}

static void loadAll(display *d) {
    loadPicture(d, "sea.bmp", 'B');
    loadPicture(d, "gridBackground.bmp", 'G');
    loadPicture(d, "testSquare.bmp", 'T');
    loadPicture(d, "S.bmp", 'S');
    loadPicture(d, "R.bmp", 'R');
    loadPicture(d, "hitShip.bmp", 'X');
    loadPicture(d, "water.bmp", 'W');
    loadPicture(d, "unknown.bmp", 'N');
    loadPicture(d, "sunkShip.bmp", 'U');
    loadPicture(d, "aimTest.bmp", 'A');
    loadPicture(d, "aimTest.bmp", 'M');
    //Front
    loadPicture(d, "RF.bmp", 'E');
    loadPicture(d, "SF.bmp", 'F');
    //Rear
    loadPicture(d, "RR.bmp", 'e');
    loadPicture(d, "SR.bmp", 'f');


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

static void updateGrid(display *d, grid *g, field **gridMatrix, int position) {
    g->gridMatrix = gridMatrix;
    //if ((d->prevWidth == d->width) && (d->prevHeight == d->height)) return;
    int screenW = d->width;
    int screenH = d->height;
    int squareW = screenW * 4 / 100;
    int squareH = screenH * 8 / 100;
    if (squareW < squareH) squareH = squareW;
    else squareW = squareH;
    // printf("sqW = %d sqH = %d\n", squareW, squareH);
    g->squareW = squareW;
    g->squareH = squareH;
    g->space = 0;
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
    d->prevWidth = screenW;
    d->prevHeight = screenH;
    //g->selectedCol = 0;
    //g->selectedRow = 0;
}

grid *newGrid(display *d, field **gridMatrix, int position) {
    grid *g = malloc(sizeof(grid));
    updateGrid(d, g, gridMatrix, position);
    g->selected = false;
    g->confirmed = false;
    g->selectedCol = 0;
    g->selectedRow = 0;
    return g;
}

static void placeOne(display *d, grid *g, int field, int x, int y) {
    char c = -1;
	if (field == S) c = 'S';
    if (field == R) c = 'R';
    if (field == SF) c = 'F';
    if (field == RF) c = 'E';
    if (field == SR) c = 'f';
    if (field == RR) c = 'e';
	if (field == X) c = 'X';
	if (field == W) c = 'W';
	if (field == N) c = 'N';
	if (field == U) c = 'U';
    //printf("char c = %c\n\n", c);
    placePicture(d, c, x, y, g->squareW, g->squareH);
}

static void placeAim(display *d, grid *g, int row, int col, int x, int y) {
    bool A = g->position == 2;
    bool B = g->selectedCol == col;
    bool C = g->selectedRow == row;
    if (A && B && C) {
        placePicture(d, 'A', x, y, g->squareW, g->squareH);
    }
}

static void placeShip(display *d, grid *g, int row, int col, int x, int y) {
    //printf("inside placeship\n orient = %c\n", g->orientation);
    bool A = g->position == 1;
    bool B = false;
    if (g->orientation == 'v') B = (row >= g->selectedRow) && (row <  g->selectedRow + g->length);
    if (g->orientation == 'h') B = (col >= g->selectedCol) && (col <  g->selectedCol + g->length);
    bool C = false;
    if (g->orientation == 'v') C = g->selectedCol == col;
    if (g->orientation == 'h') C = g->selectedRow == row;
    //printf("A %d B %d C %d\n", A, B, C);
    if (A && B && C) {
        placePicture(d, 'M', x, y, g->squareW, g->squareH);
    }
}

void placeGrid(display *d, grid *g, bool aiming, bool placing) {
    if(g == NULL) return;
    // printf("aiming %d , placing %d\n", aiming, placing);
    updateGrid(d, g, g->gridMatrix, g->position);
    placePicture(d, 'G', g->x, g->y, g->gridW, g->gridH);
    int row = 0;
    int col = 0;
    while (row < 10) {
        col = 0;
        while (col < 10) {
            int field = g->gridMatrix[col][row];
            //printf("field: %d\n", field);
            int x = g->x + g->space + col * (g->squareW + g->space);
            //printf("x %d\n", x);
            int y = g->y + g->space + row * (g->squareH + g->space);
            //printf("y %d\n", y);
            placeOne(d, g, field, x, y);
            if (aiming) placeAim(d, g, row, col, x, y);
            if (placing) placeShip(d, g, row, col, x, y);
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
    d->renderer = notNull(SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED));

    notNeg(SDL_RenderClear(d->renderer));
    //loadPicture(d, "sea.bmp", 0, 0, d->width, d->height);
    //loadPicture(d, "gridBackground.bmp", 10, 10, 365, 365);
    //loadPicture(d, "testSquare.bmp", 15, 15, 30, 30);
    //loadPicture(d, "testSquare.bmp", 50, 15, 30, 30);
    //SDL_RenderPresent(d->renderer);
    loadAll(d);
    return d;
}

void updateDisplay(display *d, grid *g1, grid *g2, bool aiming, bool placing) {
	placeBackground(d);
	placeGrid(d, g1, aiming, placing);
	placeGrid(d, g2, aiming, placing);
	displayFrame(d);
}

// Note that after resizing all grids have to be updated using placeGrid
static void resizeWindow(display *d, int w, int h) {
    d->prevWidth = d->width;
    d->prevHeight = d->height;
    d->width = w;
    d->height = h;
}

void pause(display *d, int ms) {
    if (ms > 0) SDL_Delay(ms);
}

void end(display *d) {
    pause(d, 2000);
    //SDL_FreeSurface(d->surface);
    //SDL_DestroyRenderer(d->renderer);
    //SDL_DestroyWindow(d->window);
    SDL_Quit();
}
//-----------------------------------------------------------------------------
// COORDINATE SELECTION

// 'Selected' shows whether events match the selection
// 'Confirmed' shows whether the player confirmed the target

static void moveSelection(grid *g, char o, int delta) {
    int maxRow = 9;
    int maxCol = 9;
    int minRow = 0;
    int minCol = 0;

    if (g->orientation == 'h') maxCol = 10 - g->length;
    else maxRow = 10 - g->length;

    if(o == 'r') {
        int new = g->selectedRow + delta;
        if ((new <= maxRow) && (new >= minRow)) g->selectedRow = new;
        else if (new > maxRow) g->selectedRow = maxRow;
        else if (new < minRow) g->selectedRow = minRow;
    }
    if(o == 'c') {
        int new = g->selectedCol + delta;
        if ((new <= maxCol) && (new >= minCol)) g->selectedCol = new;
        else if (new > maxCol) g->selectedCol = maxCol;
        else if (new < minCol) g->selectedCol = minCol;
    }
}

static void swapOrientation(grid *g) {
    if (g->orientation == 'h') g->orientation = 'v';
    else if (g->orientation == 'v') g->orientation = 'h';

    moveSelection(g, 'r', 0);
    moveSelection(g, 'c', 0);
}

static void checkCoords(int row, int col, int x, int y, grid *g) {
    int startX = g->x + g->space + row * (g->squareW + g->space);
    int endX = startX + g->squareW;
    //printf("x %d\n", x);
    int startY = g->y + g->space + col * (g->squareH + g->space);
    int endY = startY + g->squareH;
    //printf("y %d\n", y);
    if ((x >= startX) && (x <= endX) && (y >= startY) && (y <= endY)) {
        g->selectedCol = row;
        // printf("x selected as %d\n", g->selectedCol);
        g->selectedRow = col;
        // printf("y selected as %d\n", g->selectedRow);
        moveSelection(g, 'r', 0);
        moveSelection(g, 'c', 0);
        g->selected = true;
    }
}

static void setCoordsMouse(int x, int y, grid *g) {
    // printf("setting coords with mouse\n");
    // printf("mouse says x = %d , y = %d\n", x, y);
    g->selected = false;
    checkCoords(g->selectedRow, g->selectedCol, x, y, g);
    if (g->selected) return;
    // printf("first check not successfull\n");
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
    // printf("Done loop, selected = %d\n", g->selected);
}

void forgetEvents(display *d) {
    SDL_Event event_structure;
    SDL_Event *event = &event_structure;
    while (SDL_PollEvent(event));
    // printf("Done forgetting");
}


static bool setCoords(display *d, grid *g) {
    g->confirmed = false;
    g->save = false;
    SDL_Event event_structure;
    SDL_Event *event = &event_structure;
    notNeg(SDL_WaitEvent(event));

    if (event->type == SDL_QUIT) {
        SDL_Quit();
        exit(0);
    }
    else if (event->type == SDL_KEYDOWN) {
        int sym = event->key.keysym.sym;
        g->selected = true;
        if ((sym == SDLK_UP) || (sym == SDLK_w)) moveSelection(g, 'r', -1);
        if ((sym == SDLK_DOWN) || (sym == SDLK_s)) moveSelection(g, 'r', 1);
        if ((sym == SDLK_LEFT) || (sym == SDLK_a)) moveSelection(g, 'c', -1);
        if ((sym == SDLK_RIGHT) || (sym == SDLK_d)) moveSelection(g, 'c', 1);
        if (sym == SDLK_r) swapOrientation(g);
        if (sym == SDLK_p) g->save = true;
        if ((sym == SDLK_SPACE)) {
            g->confirmed = true;
            g->selected = false;
        }
    }
    else if (event->type == SDL_MOUSEMOTION) {
        setCoordsMouse(event->motion.x, event->motion.y, g);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN) {
        // printf("detected button down, selected says %d\n", g->selected);
        //if ((event->button.button == SDL_BUTTON_LEFT) && g->selected) g->confirmed = true;
        if (g->selected) {
            g->confirmed = true;
            g->selected = false;
        }
    }
    else if (event->type == SDL_WINDOWEVENT) {
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            resizeWindow(d, event->window.data1, event->window.data2);
        }
    }
    return g->confirmed;
}

bool setShipLocation(display *d, grid *g, int length, char *orientation) {
    g->orientation = orientation[0];
    g->length = length;
    return setCoords(d, g);
}

bool setAim(display *d, grid *g) {
    g->length = 1;
    return setCoords(d, g);
}

int getXcoord(grid *g) {
    return g->selectedCol;
}

int getYcoord(grid *g) {
    return g->selectedRow;
}

bool getSave(grid *g) {
    return g->save;
}

char getOrientation(grid *g) {
    return g->orientation;
}

//-----------------------------------------------------------------------------
// MESSAGE BOX

void displayMessage(display *d, char *title, char* message) {
    notNeg(SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, d->window));
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

static void messageTest() {
    display *d = newDisplay("message test", 1000, 500);
    displayMessage(d, "Testing title", "Message test");
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
    messageTest();
    printf("All tests DONE\n");
}

int displayMain() {
    tests();
    return 0;
}
