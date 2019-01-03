#include "Computer.h"

int dx[numDirections] = { 0, 1, 0, -1 };
int dy[numDirections] = { 1, 0, -1, 0 };

bool isAddressValid(int address) {
  return address >= 0 && address < memorySize;
}

Computer::Computer(int size) {
  _size = size;

  clear();
  reset();
}

void Computer::clear() {
  for (int x = 0; x < _size; x++) {
    for (int y = 0; y < _size; y++) {
      _program[x][y] = Instruction::Noop;
    }
  }
}

void Computer::reset() {
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
      for (int d = 0; d < numDirections; d++) {
        _exitCount[x][y][d] = 0;
      }
    }
  }
}

void Computer::setProgram(int* program) {
  for (int x = 0; x < _size; x++) {
    for (int y = 0; y < _size; y++) {
      _program[x][y] = (Instruction)program[x + (_size - y - 1) * _size];
    }
  }
}

bool Computer::step() {
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

    switch (_program[x][y]) {
      case Instruction::Data:
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

      case Instruction::Turn:
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

      case Instruction::Noop: // Do nothing
        break;
    }
  } while (_dir != oldDir);

  if (_numSteps++ > 0) {
    _exitCount[_x][_y][(int)_dir]++;
  }

  _x = x;
  _y = y;

  return (_status == Status::Running);
}

