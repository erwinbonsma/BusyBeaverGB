/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "Computer.h"

const int8_t dx[numDirections] = { 0, 1, 0, -1 };
const int8_t dy[numDirections] = { 1, 0, -1, 0 };

Computer::Computer() {
  _size = maxProgramSize; // Default

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
  _dp = 0;
  _dp_min = 0;
  _dp_max = 0;

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

void Computer::setSize(uint8_t size) {
  _size = size;

  clear();
  reset();
}

bool Computer::step() {
  if (hasTerminated()) {
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
            _data[(_dp + dataSize) % dataSize]++;
            break;
          case Direction::Down:
            _data[(_dp + dataSize) % dataSize]--;
            break;
          case Direction::Right:
            _dp++;
            _dp_max = max(_dp_max, _dp);
            if ((_dp_max - _dp_min + 1) >= dataSize) {
              _status = Status::Error;
            }
            break;
          case Direction::Left:
            _dp--;
            _dp_min = min(_dp_min, _dp);
            if ((_dp_max - _dp_min + 1) >= dataSize) {
              _status = Status::Error;
            }
            break;
        }
        break;

      case Instruction::Turn:
        if (_data[(_dp + dataSize) % dataSize] != 0) {
          pp.dir = (Direction)(((int)pp.dir + 1) % 4);
        } else {
          pp.dir = (Direction)(((int)pp.dir + 3) % 4);
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
