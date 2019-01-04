#include "Challenges.h"

#include <Gamebuino-Meta.h>

#include "Computer.h"
#include "Drawing.h"

enum class Comparison : int {
  Equals = 0,
  GreaterThan = 1,
  LessThan = 2
};

char comparisonChars[3] = { '=', '>', '<' };

class ExitChallenge : Challenge {
  int _exitX, _exitY;
public:
  ExitChallenge(int exitX, int exitY);

  void draw();
  bool isAchieved(Computer& computer);
};

class ComparisonBasedChallenge : Challenge {
  int _target;
  Comparison _comparison;

protected:
  bool isAchieved(int value);

public:
  void draw();
  ComparisonBasedChallenge(int target, Comparison comparison);
};

class OutputChallenge : ComparisonBasedChallenge {
public:
  void draw();
  bool isAchieved(Computer& computer);
};

class RunLengthChallenge : ComparisonBasedChallenge {
public:
  void draw();
  bool isAchieved(Computer& computer);
};

//--------------------------------------------------------------------------------------------------
// ExitChallenge

ExitChallenge::ExitChallenge(int exitX, int exitY) {
  _exitX = exitX;
  _exitY = exitY;
}

void ExitChallenge::draw() {
  int x = getDisplayX(_exitX);
  int y = getDisplayY(_exitY);

  // Draw sign
  gb.display.setColor(GREEN);
  gb.display.fillRect(x - 1, y - 1, 7, 7);

  gb.display.setCursor(0, 0);
  gb.display.print("EXIT");

  // Draw core shape of arrow
  gb.display.setColor(WHITE);
  gb.display.drawLine(x, y + 2, x + 4, y + 2);
  gb.display.drawLine(x + 2, y, x + 2, y + 4);

  // Draw orientation-specific part of arrow
  if (x < 0) {
    gb.display.drawLine(x + 1, y + 1, x + 1, y + 3);
  }
  else if (y < 0) {
    gb.display.drawLine(x + 1, y + 3, x + 3, y + 3);
  }
  else if (x == maxProgramSize) {
    gb.display.drawLine(x + 3, y + 1, x + 3, y + 3);
  }
  else if (y == maxProgramSize) {
    gb.display.drawLine(x + 1, y + 1, x + 3, y + 1);
  }
}

bool ExitChallenge::isAchieved(Computer& computer) {
  ProgramPointer pp = computer.getProgramPointer();

  return (pp.x == _exitX && pp.y == _exitY);
}

//--------------------------------------------------------------------------------------------------
// ComparisonBasedChallenge

ComparisonBasedChallenge::ComparisonBasedChallenge(int target, Comparison comparison) {
  _target = target;
  _comparison = comparison;
}

void ComparisonBasedChallenge::draw() {
  gb.display.setCursor(0, 0);
  gb.display.printf("# %c %d", comparisonChars[(int)_comparison], _target);
}

bool ComparisonBasedChallenge::isAchieved(int value) {
  switch (_comparison) {
    case Comparison::Equals:
      return value == _target;
    case Comparison::GreaterThan:
      return value > _target;
    case Comparison::LessThan:
      return value < _target;
    default:
      return false;
  }
}

//--------------------------------------------------------------------------------------------------
// OutputChallenge

void OutputChallenge::draw() {
  gb.display.setColor(GREEN);
  ComparisonBasedChallenge::draw();
}

bool OutputChallenge::isAchieved(Computer& computer) {
  int dp = computer.getDataPointer();

  if (!computer.isDataAddressValid(dp)) {
    return false;
  }
  else {
    return ComparisonBasedChallenge::isAchieved(computer.getData(dp));
  }
}

//--------------------------------------------------------------------------------------------------
// RunLengthChallenge

void RunLengthChallenge::draw() {
  gb.display.setColor(BLUE);
  ComparisonBasedChallenge::draw();
}

bool RunLengthChallenge::isAchieved(Computer& computer) {
  return ComparisonBasedChallenge::isAchieved(computer.getNumSteps());
}


