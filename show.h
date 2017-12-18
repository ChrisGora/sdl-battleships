/* A simple module designed to either print a message or show a message box,
    depending on wether the game is played in terminal or using the display. */

#include "state.h"
#include <stdbool.h>

void showMessage(bool useDisplay, display *d, char* title, char* message, bool printTitle);
