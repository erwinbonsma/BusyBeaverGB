#include "Challenges.h"

#include "Computer.h"
#include "Drawing.h"

enum class Comparison : int {
  Equals = 0,
  GreaterThan = 1,
  LessThan = 2
};

char comparisonChars[3] = { '=', '>', '<' };

int activeChallenge = NO_CHALLENGE;

//--------------------------------------------------------------------------------------------------
// ChallengeGoal definitions

class ChallengeGoal {
public:
  virtual void draw() = 0;
  virtual bool isAchieved(Computer& computer) = 0;
};

class ExitChallenge : public ChallengeGoal {
  int _exitX, _exitY;
public:
  ExitChallenge(int exitX, int exitY);

  void draw();
  bool isAchieved(Computer& computer);
};

class ComparisonBasedChallenge : public ChallengeGoal {
  int _target;
  Comparison _comparison;

protected:
  bool isAchieved(int value);

public:
  ComparisonBasedChallenge(int target, Comparison comparison);

  void draw();
};

class OutputChallenge : public ComparisonBasedChallenge {
public:
  OutputChallenge(int target, Comparison comparison);

  void draw();
  bool isAchieved(Computer& computer);
};

class RunLengthChallenge : public ComparisonBasedChallenge {
public:
  void draw();
  bool isAchieved(Computer& computer);
};

//--------------------------------------------------------------------------------------------------

#define TURN 0
#define DATA 128
const uint8_t fixedLevel2[4] = { 6|TURN, 10|TURN, 52|TURN, 54|TURN };
const uint8_t fixedLevel6[9] = {
  3|TURN, 17|TURN, 24|TURN, 29|TURN, 41|TURN, 45|TURN, 55|TURN, 70|TURN, 76|TURN
};

const ChallengeSpec challengeSpecs[numChallenges] = {
  {
    .name = "Count to 8",
    .goal = new OutputChallenge(8, Comparison::Equals),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 0,
    .numData = 8
  },{
    .name = "Count to 12",
    .goal = new OutputChallenge(12, Comparison::Equals),
    .numFixed = 4,
    .fixed = fixedLevel2,
    .numTurn = 0,
    .numData = 12
  },{
    .name = "Exit 1",
    .goal = new ExitChallenge(4, 9),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 4,
    .numData = 1
  },{
    .name = "Exit 2",
    .goal = new ExitChallenge(1, 9),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 6,
    .numData = 1
  },{
    .name = "Exit 3",
    .goal = new ExitChallenge(1, 9),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 2,
    .numData = 2
  },{
    .name = "Exit 4",
    .goal = new ExitChallenge(2, 9),
    .numFixed = 9,
    .fixed = fixedLevel6,
    .numTurn = 0,
    .numData = 6
  },{
    .name = "Count to 16",
    .goal = new OutputChallenge(16, Comparison::Equals),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 8,
    .numData = 16
  },{
    .name = "Countdown to -8",
    .goal = new OutputChallenge(-8, Comparison::Equals),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 8,
    .numData = 10
  }
};

const Challenge challenges[numChallenges] = {
  Challenge(challengeSpecs[0]),
  Challenge(challengeSpecs[1]),
  Challenge(challengeSpecs[2]),
  Challenge(challengeSpecs[3]),
  Challenge(challengeSpecs[4]),
  Challenge(challengeSpecs[5]),
  Challenge(challengeSpecs[6]),
  Challenge(challengeSpecs[7]),
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
  if (_exitX < 0) {
    gb.display.drawLine(x + 1, y + 1, x + 1, y + 3);
  }
  else if (_exitY < 0) {
    gb.display.drawLine(x + 1, y + 3, x + 3, y + 3);
  }
  else if (_exitX == maxProgramSize) {
    gb.display.drawLine(x + 3, y + 1, x + 3, y + 3);
  }
  else if (_exitY == maxProgramSize) {
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
  gb.display.printf("#%c%d", comparisonChars[(int)_comparison], _target);
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

OutputChallenge::OutputChallenge(int target, Comparison comparison)
  : ComparisonBasedChallenge(target, comparison) {}

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

//--------------------------------------------------------------------------------------------------

Challenge::Challenge(const ChallengeSpec& spec) : _spec(spec) {}

void Challenge::setFixedInstructions(Computer& computer) const {
  for (int i = 0; i < _spec.numFixed; i++) {
    int insCode = _spec.fixed[i];
    Instruction instruction = (insCode & DATA) ? Instruction::Data : Instruction::Turn;
    insCode &= 127; // Clear instruction bit
    int fixX = insCode % 9;
    int fixY = (insCode - fixX) / 9;
    computer.setInstruction(fixX, fixY, instruction);
  }
}

bool Challenge::isFixed(int x, int y) const {
  for (int i = 0; i < _spec.numFixed; i++) {
    int insCode = (_spec.fixed[i] & 127);
    int fixX = insCode % 9;
    int fixY = (insCode - fixX) / 9;
    if (x == fixX && y == fixY) {
      return true;
    }
  }
  return false;
}

int Challenge::numAvailable(Instruction instruction) const {
  switch (instruction) {
    case Instruction::Turn:
      return _spec.numTurn;
    case Instruction::Data:
      return _spec.numData;
    default:
      return 0;
  }
}

void Challenge::draw() const {
  _spec.goal->draw();
}

bool Challenge::isAchieved(Computer& computer) const {
  return _spec.goal->isAchieved(computer);
}

