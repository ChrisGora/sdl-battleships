
#include "show.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void showMessage(bool useDisplay, display *d, char* title, char* message, bool printTitle) {
	if (useDisplay) displayMessage(d, title, message);
	else {
		if (printTitle) printf("%s\n", title);
		printf("%s\n", message);
	}
}

int showMain() {
    display *d = newDisplay("show test", 100, 100);
    showMessage(true, d, "test1", "message test1", true);
    showMessage(false, d, "test2", "message test2", true);
    showMessage(false, d, "test3", "message test3", false);
    printf("All tests passed\n");
    return 0;
}
