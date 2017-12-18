# BATTLESHIPS #
-- Version 2 --


### MAIN CHANGES

The game can now be played with full graphics. Also the game now supports saving
and loading of a previously saved game.


### NORMAL USAGE

A Makefile has been included.

Start a new game using graphics:
./battle d

Load a previously saved game (only graphics):
./battle l

Start new game in terminal (barely any changes, no support for saving/loading):
./battle


### DEBUGGING

In order to run tests use:
./battle t

NOTE that the save.battle file will be completely overwritten as part of testing


### ABOUT THE GRAPHICS

Healthy ships get displayed 'nicely' - they have a front, a rear, a middle and
they do get displayed as horizontal or vertical - ie. they look like actual ships.

When a ship has been hit a simple yellow square is shown. This is to avoid the
player knowing which direction the ship is facing.

The window is fully resizable. This was achieved through the grid dimensions
being a function of square dimensions which in turn were calculated from the
window dimensions. This means that whenever the window is resized, the grids
will also resize. It's also worth noting that mouse selection does not break
when the window is resized!

When aiming, placing and even rotating there is no way for a ship to end up
outside of the grid. In case of trying to place two ships on top of each other
a message box is shown.

As you might remember, I came by your office on Friday to sort out the SDL event
queue and I can confirm that, as you suggested, a very simple one-line loop
solved all my problems:

while (SDL_PollEvent(event));


### KEYBINDINGS

Move selection (when aiming or placing a ship):
Arrow Keys, WSAD or the mouse

Confirm selection:
Space or any mouse button

Rotate ship (when placing a new ship only)
r

Save game (when aiming only - saving does not work during initial game setup)
p


### MODULES AND STRUCTURE

battle  - The main game file which runs all logic and terminal IO - the controller
show    - A module for showing the user some informastion in the correct way
        (terminal or display). This replaced various printfs in battle.
state   - Module to store and manage the main game structure
display - The display module
field   - A tiny header with the field enum


Based strictly on the headers, my dependencies work as follows:

battle -> show -> state -> display -> field

There are no cyclic dependencies. Each module depends on most modules on its right,
but none to its left.

This isn't the best structure in the world, but it works reasonably well in this
case, as there are a lot of structures and many modules need access to them.
For example:
- show module needs access to display, a pointer to which comes from state.
- field is required by every single module apart from show. The field enum is
    used not just internally but also across modules (in headers), which caused
    total hell when I tried to have a more branched tree.

Having said that, the 'Graphics problem' mentioned in the slides doesn't really
apply here in my opinion. All logic is easily tested using battle, state and
field only. Then the graphics are partially tested in display.c itself (creating
a display, loading pictures, etc.) but mostly in battle.c because it is required
to pass the various grids from state to display. Display cannot fetch these automatically.

Based on the way my game and esp. testing works one could argue that the
dependencies are:

battle |  -> state      |
       v                v
     show -> display -> field

The only thing false about this is that state does depend on display - it stores
a pointer to the display - however this is a very tiny connection and it doesn't
affect testing in any way. Even though that pointer is stored in state only battle
is capable of passing it from state to display. Display module has no direct access
to information in state. That's why everything is passed via battle.


### POSSIBLE IMPROVEMENTS

Battle.c is still quite huge and messy. It would be useful to separate the
terminal IO functions from logic functions. I didn't separate them in the end,
because I knew I wasn't going to work on the terminal functions anymore and also
because there are some cyclic dependencies between the IO and logic functions.
It was therefore best to leave it as it is and not break what's already working.
