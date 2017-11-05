# BATTLESHIPS #
### README ###

This is a fully working implementation of the popular Battleships game. 
Currently the only available game mode is local multiplayer.

In order to play simply compile and execute with no arguments.
In order to run tests compile and execute with one extra argument (eg ./battle t).

### How do I play? ###

RULES: https://www.hasbro.com/common/instruct/Battleship.PDF
Note that my version uses 7 ships rather than 5

### What works and what doesn't ###

The logic has been tested using autotesting. I used a wide range of tests and 
the game is fully functional.

The game ensures that you don't:
- place ships on top of each other
- place ships that go outside of the grid
- hit squares outside of grid
- hit squares that you already hit once and revealed

The game uses a structure to pass around all information.
During gameplay appropriate information about your own grids and ships is printed.
A message about the opponents previous turn is also included.
When the players swap, all information about any ships gets hidden away.

This and other IO functions allow for gameplay without exchanging a single word 
between the players (should you wish to play in such way). More importantly, 
this allows for potential singleplayer (against computer) or online multiplayer 
in the future. 

### "Screenshots" of what you might see during gameplay ###

You might not have time to play through the whole game, so below are some 
screenshots of the game at different points:

-------------------------------------------------------------------------------
1: SETTING UP YOUR SHIPS
-------------------------------------------------------------------------------

S is a healthy ship square
X is a hit ship square
# is a known water square
- is an unknown square
* is a completely sunken ship


  a b c d e f g h i j 
0 # # # # # # # # # # 
1 # S # # # # # # # # 
2 # S # # # # # # # # 
3 # S # S S S S # # # 
4 # S # # # # # # # # 
5 # S # # # # # # # # 
6 # # # # # # # # # # 
7 # # # # # # # # # # 
8 # # # # # # # # # # 
9 # # # # # # # # # # 

 


---INSTRUCTIONS---
Place a Cruiser which is 3 squares long 
Give two coordinates and press [Enter]

Type the coordinates in the form (letter)(number) - for example a5
Confirm with [Enter] - This will select a starting point for the ship
Then specify if the ship is meant to be horizontal or vertical
Based on the length of the ship, appropriate squares will be filled
EXAMPLE: When placing a Submarine (3 squares long)
typing in 'a5' followed by 'h' would fill in a5, a6 and a7


-------------------------------------------------------------------------------
2: DURING A TURN
-------------------------------------------------------------------------------

S is a healthy ship square
X is a hit ship square
# is a known water square
- is an unknown square
* is a completely sunken ship

YOUR OWN SHIPS: 

  a b c d e f g h i j 
0 # # # # # # # # # S 
1 # X # # # # # # # S 
2 # X # # # # # # # # 
3 # X # S S S S # # # 
4 # S # # # # # # # # 
5 # S # S # # # # # # 
6 # # # S # # # # # S 
7 # # # S # # # # # S 
8 # # # # # # # # # # 
9 # S S S # # S S S # 

 
WHAT YOU KNOW ABOUT ENEMY SHIPS: 

  a b c d e f g h i j 
0 - - - - - - - - - - 
1 # - # - # - - - - - 
2 - - - - - - - - - - 
3 - - - - - - - - - - 
4 - - - - - - - - - - 
5 - - - - - - - - - - 
6 - - - - - - - - - - 
7 - - - - - - - - - - 
8 - - - - - - - - - - 
9 - - - - - - - - - - 

 
Player 1 Health: 86% 
Player 2 Health: 100% 

Carrier             2/5 (40 %) 
Battleship          4/4 (100%) 
Cruiser             3/3 (100%) 
Submarine           3/3 (100%) 
second Submarine    3/3 (100%) 
Destroyer           2/2 (100%) 
second Destroyer    2/2 (100%) 


Your opponent shot at b3 and they HIT a Carrier

CALL YOUR SHOT
Please select your next target 



-------------------------------------------------------------------------------
3: AFTER SELECTING A TARGET
-------------------------------------------------------------------------------

YOUR OWN SHIPS: 

  a b c d e f g h i j 
0 # # # # # # # # # S 
1 # X # # # # # # # S 
2 # X # # # # # # # # 
3 # X # S S S S # # # 
4 # S # # # # # # # # 
5 # S # S # # # # # # 
6 # # # S # # # # # S 
7 # # # S # # # # # S 
8 # # # # # # # # # # 
9 # S S S # # S S S # 

 
WHAT YOU KNOW ABOUT ENEMY SHIPS: 

  a b c d e f g h i j 
0 - - - - - - - - - - 
1 # - # - # - - - - - 
2 - X - - - - - - - - 
3 - - - - - - - - - - 
4 - - - - - - - - - - 
5 - - - - - - - - - - 
6 - - - - - - - - - - 
7 - - - - - - - - - - 
8 - - - - - - - - - - 
9 - - - - - - - - - - 

 


You shot at b2 and you HIT a ship!

Please press [Enter] to continue


-------------------------------------------------------------------------------
4: CLEAR SCREEN FOR WHEN THE PLAYERS SWAP
-------------------------------------------------------------------------------



Up Next: Player 2 
Please press [Enter] to continue


