#include "Program.h"

int dx[numDirections] = { 0, 1, 0, -1 };
int dy[numDirections] = { 1, 0, -1, 0 };

bool isAddressValid(int address) {
  return address >= 0 && address < memorySize;
}

Program::Program(int size) {
  _size = size;

  clear();
  reset();
}

void Program::clear() {
  for (int x = 0; x < _size; x++) {
    for (int y = 0; y < _size; y++) {
      _program[x][y] = Instruction::Nop;
    }
  }
}

void Program::reset() {
  _x = 0;
  _y = -1;
  _dir = Direction::Up;
  _ptr = 0;

  _numSteps = 0;
  _status = Status::Ready;

  for (int i = 0; i < memorySize; i++) {
    _memory[i] = 0;
  }

  for (int x = 0; x < _size; x++) {
    for (int y = 0; y < _size; y++) {
      _visitCount[x][y] = 0;
    }
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
  if (_status==Status::Done || _status==Status::Error) {
    return true;
  }
  _status = Status::Running;

  int x, y;
  Direction oldDir;

  do {
    x = _x + dx[(int)_dir];
    y = _y + dy[(int)_dir];
    oldDir = _dir;

    if (x < 0 || y < 0 || x >= _size || y >= _size) {
      _status = Status::Done;
      break;
    }

    _visitCount[x][y]++;

    switch (_program[x][y]) {
      case Instruction::Mem:
        switch (_dir) {
          case Direction::Up:
            if (isAddressValid(_ptr)) {
              _memory[_ptr]++;
            } else {
              _status = Status::Error;
            }
            break;
          case Direction::Down:
            if (isAddressValid(_ptr)) {
              _memory[_ptr]--;
            } else {
              _status = Status::Error;
            }
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
        if (isAddressValid(_ptr)) {
          if (_memory[_ptr] != 0) {
            _dir = (Direction)(((int)_dir + 1) % 4);
          } else {
            _dir = (Direction)(((int)_dir + 3) % 4);
          }
        } else {
          _status = Status::Error;
        }
        break;

      case Instruction::Nop: // Do nothing
        break;
    }
  } while (_dir != oldDir);

  _numSteps++;
  _x = x;
  _y = y;

  return (_status == Status::Running);
}

