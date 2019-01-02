# Busy Beaver

This program for the [Gamebuino](https://gamebuino.com) lets you edit and run an esoteric language.
The language is based on [2L](https://esolangs.org/wiki/2L) with some minor modifications.

## Language

The language is two-dimensional.
The instructions are put on a 2D grid, whose size is limited to 9x9.
The program pointer moves across this grid, executing instructions as it encounters them.
It starts just below the left-most cell at the bottom of the grid, moving upwards.

There is also a one-dimensional tape that acts as memory.
The data pointer starts at the left-most cell of the tape.
All cells of the tape are initially filled with zeros.

The language consist of only two symbols:

*   ***TURN***, visualised by a Black square

    When the program pointer would enter a cell with a TURN instruction, it changes
    direction to prevent this.
    The direction it turns to depends on the value of the cell on the tape that the
    data pointer points to:

    *   When the value is zero, it turns ninety degrees counter-clockwise.
    *   Otherwise, it turns ninety degrees clockwise.

*   ***DATA***, visualised by a White square

    What happens when the program pointer enters a cell with a DATA instruction
    depends on the direction the program pointer is moving to:

    *   When it is moving up, it increases the value at the data pointer by one.
    *   When it is moving down, it decreases the value at the data pointer by one.
    *   When it is moving right, it moves the data pointer one position to the right.
    *   When it is moving left, it moves the data pointer one position to the left.

Program cells can also be empty, representing a NOOP instruction.
The program pointer simply keeps moving in its current direction.

The program terminates when the program pointer leaves the board,
It is an error when trying read or write to the tape when the data pointer does not point to a position on the tape.

## About the name

The project has [Busy Beaver](https://en.wikipedia.org/wiki/Busy_beaver) as working name because the main challenge is to design a program that runs for as long as possible, yet terminates.
