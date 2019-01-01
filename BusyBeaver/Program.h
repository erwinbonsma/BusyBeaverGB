

enum class Instruction : int {
  Nop = 0,
  Blk = 1,
  Mem = 2
};

enum class Direction : int {
  Up = 0,
  Right = 1,
  Down = 2,
  Left = 3
};

const int maxProgramSize = 9;
const int memorySize = 64;
const int numDirections = 4;

class Program {
  Instruction _program[maxProgramSize][maxProgramSize];
  int _size;

  // Address pointer
  int _x, _y;
  Direction _dir;

  // Memory pointer
  int _ptr;
  int _memory[memorySize];

  int _numSteps;
  bool _done;

public:
  Program(int size);

  int getSize() { return _size; }
  void setProgram(int* program);

  Instruction getInstruction(int x, int y) { return _program[x][y]; }
  void setInstruction(int x, int y, Instruction i) { _program[x][y] = i; }

  int getAddressX() { return _x; }
  int getAddressY() { return _y; }
  Direction getDirection() { return _dir; }

  // Resets the program
  void reset();

  int getMemoryAddress() { return _ptr; }
  int getMemory(int address) { return _memory[address]; }

  // Executes one instruction. Returns "true" when done
  bool step();

  int getNumSteps() { return _numSteps; }
};

