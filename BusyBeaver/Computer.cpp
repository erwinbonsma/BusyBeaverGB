#include "Computer.h"

int dx[numDirections] = { 0, 1, 0, -1 };
int dy[numDirections] = { 1, 0, -1, 0 };

bool isDataAddressValid(int address) {
  return address >= 0 && address < dataSize;
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
  _pp.x = 0;
  _pp.y = -1;
  _pp.dir = Direction::Up;
  _dp = dataSize / 2;

  _numSteps = 0;
  _status = Status::Ready;

  for (int i = 0; i < dataSize; i++) {
    _data[i] = 0;
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

  ProgramPointer pp = _pp;
  Instruction instruction;

  do {
    pp.x = _pp.x + dx[(int)pp.dir];
    pp.y = _pp.y + dy[(int)pp.dir];

    if (pp.x < 0 || pp.y < 0 || pp.x >= _size || pp.y >= _size) {
      instruction = Instruction::Noop;
      _status = Status::Done;
    }
    else {
      instruction = _program[pp.x][pp.y];
    }

    switch (instruction) {
      case Instruction::Data:
        switch (pp.dir) {
          case Direction::Up:
            if (isDataAddressValid(_dp) && _data[_dp] < maxDataValue) {
              _data[_dp]++;
            } else {
              _status = Status::Error;
            }
            break;
          case Direction::Down:
            if (isDataAddressValid(_dp) && _data[_dp] > minDataValue) {
              _data[_dp]--;
            } else {
              _status = Status::Error;
            }
            break;
          case Direction::Right:
            _dp++;
            break;
          case Direction::Left:
            _dp--;
            break;
        }
        break;

      case Instruction::Turn:
        if (isDataAddressValid(_dp)) {
          if (_data[_dp] != 0) {
            pp.dir = (Direction)(((int)pp.dir + 1) % 4);
          } else {
            pp.dir = (Direction)(((int)pp.dir + 3) % 4);
          }
        } else {
          _status = Status::Error;
        }
        break;

      case Instruction::Noop: // Do nothing
        break;
    }
  } while (_status == Status::Running && instruction == Instruction::Turn);

  if (_numSteps++ > 0) {
    _exitCount[_pp.x][_pp.y][(int)pp.dir]++;
  }

  _pp = pp;

  return (_status == Status::Running);
}

