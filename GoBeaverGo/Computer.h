/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __COMPUTER_INCLUDED
#define __COMPUTER_INCLUDED

#include <Gamebuino-Meta.h>

constexpr int numInstructions = 3;
enum class Instruction : uint8_t {
  Noop = 0,
  Turn = 1,
  Data = 2
};

constexpr int numDirections = 4;
enum class Direction : uint8_t {
  Up = 0,
  Right = 1,
  Down = 2,
  Left = 3
};

enum class Status : uint8_t {
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

struct ExecutionStats {
  uint16_t inc;
  uint16_t dec;
  uint16_t shr;
  uint16_t shl;
};

constexpr int maxProgramSize = 9;

/* The size of the data tape.
 *
 * The size should be chosen such that a well-written (terminating) program never exceeds the
 * data boundaries.
 */
constexpr int dataSize = 7000;

/* Put explicit bounds on data values.
 *
 * This is done to prevent data values from wrapping, which could cause simple non-terminating
 * programs to terminate after all.
 */
constexpr int16_t minDataValue = SHRT_MIN;
constexpr int16_t maxDataValue = SHRT_MAX;

constexpr int visitCountDim = maxProgramSize - 1;
constexpr int numVisitCounts = visitCountDim * visitCountDim * 2;

class Computer {
  // The program
  Instruction _program[maxProgramSize][maxProgramSize];

  // Tracks how often the program pointer visited each path
  uint8_t _visitCount[numVisitCounts];

  // The data (a cyclic buffer)
  int16_t (&_data)[dataSize];

  // Program pointer
  ProgramPointer _pp;

  // Execution stats
  ExecutionStats _executionStats;

  // The program's size
  uint8_t _size;

  // Execution status
  Status _status;

  // Data pointer
  int16_t _dp;

  // Bounds of the cyclic buffer
  int16_t _dp_min;
  int16_t _dp_max;

  uint32_t _numSteps;

  void shiftVisitCounts();

public:
  Computer();

  void setSize(uint8_t size);
  uint8_t getSize() const { return _size; }

  Instruction getInstruction(int x, int y) const { return _program[x][y]; }
  void setInstruction(int x, int y, Instruction i) { _program[x][y] = i; }

  uint8_t getVisitCount(int x, int y, bool horizontal) const {
    return _visitCount[(y * visitCountDim + x) * 2 + horizontal];
  }

  ProgramPointer getProgramPointer() const { return _pp; }

  // Clears the program
  void clear();

  // Resets the execution state
  void reset();

  void resetExecutionStats() { _executionStats = ExecutionStats(); }
  ExecutionStats getExecutionStats() const { return _executionStats; }

  int getMinDataAddress() const { return _dp_min; }
  int getMaxDataAddress() const { return _dp_max; }
  int getDataPointer() const { return _dp; }
  int getData(int address) const { return _data[(address + dataSize) % dataSize]; }
  int getOutput() const { return getData(getDataPointer()); }

  // Executes one instruction. Returns "true" if the program is still running.
  bool step();

  bool hasTerminated() const { return _status==Status::Done || _status==Status::Error; }
  Status getStatus() const { return _status; }
  uint32_t getNumSteps() const { return _numSteps; }
};

#endif
