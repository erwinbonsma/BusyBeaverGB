# Busy Beaver

This program for the [Gamebuino](https://gamebuino.com) lets you edit and run an esoteric programming language.
The language is based on [2L](https://esolangs.org/wiki/2L) with some minor modifications.

## Language

The language is two-dimensional.
The instructions are put on a 2D grid, whose size is limited to 9x9.
The directional program pointer (PP) moves across this grid, executing instructions as it encounters them.
It starts just below the left-most cell at the bottom of the grid, moving upwards.

There is also a one-dimensional, infinite tape that acts as data.
All cells of the tape are initially filled with zeros.
The data pointer (DP) points to one of these cells.

The language consist of only two symbols:

*   **TURN**, visualised by a Black square

    When PP would enter a cell with a TURN instruction, it changes direction to prevent this.
    The direction it turns to depends on the value that DP points to:

    *   When the value is zero, PP turns ninety degrees counter-clockwise.
    *   Otherwise, PP turns ninety degrees clockwise.

*   **DATA**, visualised by a White square

    What happens when PP enters a cell with a DATA instruction depends on the direction of PP:

    *   When it is moving up, it increases the value at DP by one.
    *   When it is moving down, it decreases the value at DP by one.
    *   When it is moving right, it moves DP one position to the right.
    *   When it is moving left, it moves DP one position to the left.

Program cells can also be empty, representing a NOOP instruction;
PP simply keeps moving in its current direction.

The program terminates when PP leaves the board.

## About the name

The project has [Busy Beaver](https://en.wikipedia.org/wiki/Busy_beaver) as working name because the main challenge is to design a program that runs for as long as possible, yet terminates.
