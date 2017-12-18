/* This module provides a field enum, used by all the other modules */

// S is a ship that hasn't been hit (Horizontal)
// R is a ship that hasn't been hit (Vertical)
// SF - Horizontal ship front
// SR - Horizontal ship rear
// RF - Vertical ship front
// RR - Vertical ship rear
// X is a ship that has been hit
// W is known water
// N is unknown
// U is a completely sunk ship

enum field {S, X, W, N, U, R, SF, RF, SR, RR};
typedef enum field field;
