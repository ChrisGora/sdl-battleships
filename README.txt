# BATTLESHIPS #
### README ###

This is a fully working implementation of the popular Battleships game. Currently the only available game mode is local multiplayer.

In order to play simply compile and execute with no arguments.
In order to run tests compile and execute with one extra argument (eg ./battle t).

### How do I play? ###

RULES: https://www.hasbro.com/common/instruct/Battleship.PDF
Note that my version uses 7 ships rather than 5

### What works and what doesn't ###

The logic has been tested using autotesting. I used a wide range of tests and the game is fully functional.
The game ensures that you don't:
- place ships on top of each other
- place ships that go outside of the grid
- hit squares outside of grid
- hit squares that you already hit once and revealed

The game uses a structure to pass around all information.
During gameplay appropriate information about your own grids and ships is printed.
A message about the opponents previous turn is also included.
When the players swap, all information about any ships gets hidden away.

This and other IO functions allow for gameplay without exchanging a single word between the players (should you wish to play in such way).
More importantly, this allows for potential singleplayer (against computer) or online multiplayer in the future. 

### "Screenshots" of what you might see during gameplay ###

You might not have time to play through the whole game, so below are some screenshots of the game at different points:

