#include "Program.h"

int dx[numDirections] = { 0, 1, 0, -1 };
int dy[numDirections] = { 1, 0, -1, 0 };

Program::Program(int size) {
  _size = size;

  for (int x = 0; x < size; x++) {
    for (int y = 0; y < size; y++) {
      _program[x][y] = Instruction::Nop;
    }
  }

  reset();
}

void Program::reset() {
  _x = 0;
  _y = -1;
  _dir = Direction::Up;
  _numSteps = 0;

  for (int i = 0; i < memorySize; i++) {
    _memory[i] = 0;
  }
}

void Program::setProgram(int* program) {
  for (int x = 0; x < _size; x++) {
    for (int y = 0; y < _size; y++) {
      _program[x][y] = (Instruction)program[x + (_size - y - 1) * _size];
    }
  }
}

bool Program::step() {
  int x, y;
  Direction oldDir;

  do {
    x = _x + dx[(int)_dir];
    y = _y + dy[(int)_dir];
    oldDir = _dir;

    if (x < 0 || y < 0 || x >= _size || y >= _size) {
      return true;
    }

    switch (_program[x][y]) {
      case Instruction::Mem:
        switch (_dir) {
          case Direction::Up:
            _memory[_ptr]++;
            break;
          case Direction::Down:
            _memory[_ptr]--;
            break;
          case Direction::Right:
            _ptr++;
            break;
          case Direction::Left:
            _ptr--;
            break;
        }
        break;

      case Instruction::Blk:
        if (_memory[_ptr] > 0) {
          _dir = (Direction)(((int)_dir + 1) % 4);
        } else {
          _dir = (Direction)(((int)_dir + 3) % 4);
        }
        break;

      case Instruction::Nop: // Do nothing
        break;
    }
  } while (_dir != oldDir);

  _numSteps++;
  _x = x;
  _y = y;

  return false;
}

