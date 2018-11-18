# SDL BATTLESHIPS
This is a fully working implementation of the popular Battleships game in C. The graphics uses SDL. Currently the only available game mode is local multiplayer.

### How do I play?

RULES: https://www.hasbro.com/common/instruct/Battleship.PDF
Note that my version uses 7 ships rather than 5

### Normal usage

A Makefile has been included.

Start a new game using graphics:
`./battle d`

Load a previously saved game (only graphics):
`./battle l`

Start new game in terminal (Old version with no support for saving/loading):
`./battle`

### Debugging

In order to run tests use:
`./battle t`

NOTE that the save.battle file will be completely overwritten as part of testing


### What works and what doesn't

The logic has been tested using autotesting. I used a wide range of tests and the game is fully functional.

The game ensures that you don't:

* place ships on top of each other
* place ships that go outside of the grid
* hit squares outside of grid
* hit squares that you already hit once and revealed

The game uses a struct to pass around all information. During gameplay appropriate information about your own grids and ships is shown. A message about the opponents previous turn is also included. When the players swap, all information about any ships gets hidden away.

The game allows for gameplay without exchanging a single word between the players (should you wish to play in such way). More importantly, this allows for potential singleplayer (against computer) or online multiplayer in the future.

### About the graphics

Healthy ships get displayed 'nicely' - they have a front, a rear, a middle and they do get displayed as horizontal or vertical - ie. they look like actual ships.

When a ship has been hit a simple yellow square is shown. This is to avoid the player knowing which direction the ship is facing.

The window is fully resizable. This was achieved through the grid dimensions being a function of square dimensions which in turn were calculated from the window dimensions. This means that whenever the window is resized, the grids will also resize. It's also worth noting that mouse selection does not break when the window is resized!

When aiming, placing and even rotating there is no way for a ship to end up outside of the grid. In case of trying to place two ships on top of each other a message box is shown.

### Keybindings

Move selection (when aiming or placing a ship):
`Arrow Keys, WSAD or the mouse`

Confirm selection:
`Space or any mouse button`

Rotate ship (when placing a new ship only)
`r`

Save game (when aiming only - saving does not work during initial game setup)
`p`


### Modules and structure

**battle**

The main game file which runs all logic and terminal IO - the controller

**show**

A module for showing the user some informastion in the correct (terminal or display). This replaced various printfs in battle.

**state**

Module to store and manage the main game structure

**display**

The display module

**field**

A tiny header with the field enum

Based strictly on the headers, my dependencies work as follows:

`battle -> show -> state -> display -> field`

There are no cyclic dependencies. Each module depends on most modules on its right, but none to its left.

This isn't the best structure in the world, but it works reasonably well in this case, as there are a lot of structures and many modules need access to them.

For example:

* show module needs access to display, a pointer to which comes from state.
* field is required by every single module apart from show. The field enum is used not just internally but also across modules (in headers), which caused total hell when I tried to have a more branched tree.


### POSSIBLE IMPROVEMENTS

Battle.c is still quite huge and messy (reason being that the first version was strictly terminal based). It would be useful to separate the terminal IO functions from logic functions a bit more. I didn't separate them in the end, because I knew I wasn't going to need the terminal functions anymore and also because there are some cyclic dependencies between the IO and logic functions. It was therefore best to leave it as it is and not break what's already working.
