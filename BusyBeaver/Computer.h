#include <limits.h>

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

const int maxProgramSize = 9;

/* The size of the data tape.
 *
 * The size should be chosen such that a well-written (terminating) program never exceeds the
 * data boundaries.
 */
const int memorySize = 64;

/* Put explicit bounds on data values.
 *
 * This is done to prevent data values from wrapping, which could cause simple non-terminating
 * programs to terminate after all.
 */
const int minDataValue = SHRT_MIN;
const int maxDataValue = SHRT_MAX;

class Computer {
  Instruction _program[maxProgramSize][maxProgramSize];
  int _size;

  // Tracks how often the program pointer exited in the given direction
  int _exitCount[maxProgramSize][maxProgramSize][numDirections];

  // Address pointer
  int _x, _y;
  Direction _dir;

  // Memory pointer
  int _ptr;
  int _memory[memorySize];

  Status _status;
  int _numSteps;

public:
  Computer(int size);

  int getSize() { return _size; }
  void setProgram(int* program);

  Instruction getInstruction(int x, int y) { return _program[x][y]; }
  void setInstruction(int x, int y, Instruction i) { _program[x][y] = i; }

  int getExitCount(int x, int y, Direction d) { return _exitCount[x][y][(int)d]; }

  int getAddressX() { return _x; }
  int getAddressY() { return _y; }
  Direction getDirection() { return _dir; }

  // Clears the program
  void clear();

  // Resets the execution state
  void reset();

  int getMemoryAddress() { return _ptr; }
  int getMemory(int address) { return _memory[address]; }

  // Executes one instruction. Returns "true" if the program is still running.
  bool step();

  Status getStatus() { return _status; }
  int getNumSteps() { return _numSteps; }
};

