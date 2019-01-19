/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __COMPUTER_INCLUDED
#define __COMPUTER_INCLUDED

#include <Gamebuino-Meta.h>

const int numInstructions = 3;
enum class Instruction : int {
  Noop = 0,
  Turn = 1,
  Data = 2
};

const int numDirections = 4;
enum class Direction : int {
  Up = 0,
  Right = 1,
  Down = 2,
  Left = 3
};

enum class Status : int {
  Ready = 0,
  Running = 1,
  Done = 2,
  Error = 3
};

struct ProgramPointer {
  int8_t x;
  int8_t y;
  Direction dir;
};

const int maxProgramSize = 9;

/* The size of the data tape.
 *
 * The size should be chosen such that a well-written (terminating) program never exceeds the
 * data boundaries.
 */
const int dataSize = 256;

/* Put explicit bounds on data values.
 *
 * This is done to prevent data values from wrapping, which could cause simple non-terminating
 * programs to terminate after all.
 */
const int minDataValue = SHRT_MIN;
const int maxDataValue = SHRT_MAX;

class Computer {
  // The program
  Instruction _program[maxProgramSize][maxProgramSize];
  int _size;

  // Tracks how often the program pointer exited in the given direction
  int _exitCount[maxProgramSize][maxProgramSize][numDirections];

  // Program pointer
  ProgramPointer _pp;

  // The data (a cyclic buffer)
  int _data[dataSize];

  // Data pointer
  int _dp;

  // Bounds of the cyclic buffer
  int _dp_min;
  int _dp_max;

  Status _status;
  int _numSteps;

public:
  Computer();

  void setSize(int size);
  int getSize() { return _size; }

  Instruction getInstruction(int x, int y) { return _program[x][y]; }
  void setInstruction(int x, int y, Instruction i) { _program[x][y] = i; }

  int getExitCount(int x, int y, Direction d) { return _exitCount[x][y][(int)d]; }

  ProgramPointer getProgramPointer() { return _pp; }

  // Clears the program
  void clear();

  // Resets the execution state
  void reset();

  int getMinDataAddress() { return _dp_min; }
  int getMaxDataAddress() { return _dp_max; }
  int getDataPointer() { return _dp; }
  int getData(int address) { return _data[(address + dataSize) % dataSize]; }
  int getOutput() { return getData(getDataPointer()); }

  // Executes one instruction. Returns "true" if the program is still running.
  bool step();

  bool hasTerminated() { return _status==Status::Done || _status==Status::Error; }
  Status getStatus() { return _status; }
  int getNumSteps() { return _numSteps; }
};

#endif

