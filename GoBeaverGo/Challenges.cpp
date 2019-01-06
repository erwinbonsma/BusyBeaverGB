/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

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
  virtual const char* typeDescription() = 0;
  virtual void draw() = 0;
  virtual bool isAchieved(Computer& computer) = 0;
};

class ExitGoal : public ChallengeGoal {
  int _exitX, _exitY;
public:
  ExitGoal(int exitX, int exitY);

  const char* typeDescription() { return "Reach the Exit"; }
  void draw();
  bool isAchieved(Computer& computer);
};

class ComparisonBasedGoal : public ChallengeGoal {
  char _targetVar;
  int _targetVal;
  Comparison _comparison;

protected:
  bool isAchieved(int value);

public:
  ComparisonBasedGoal(char targetVar, int targetVal, Comparison comparison);

  void draw();
};

class OutputValueGoal : public ComparisonBasedGoal {
public:
  OutputValueGoal(int target, Comparison comparison);

  const char* typeDescription()  { return "Output Value"; }
  void draw();
  bool isAchieved(Computer& computer);
};

class RunLengthGoal : public ComparisonBasedGoal {
public:
  RunLengthGoal(int target, Comparison comparison);

  const char* typeDescription() { return "Run Length"; }
  void draw();
  bool isAchieved(Computer& computer);
};

class SequenceGoal : public ChallengeGoal {
  int _len;
  const int8_t* _sequence;

  int drawDots(int x);
  int drawNumber(int num, int x);

public:
  SequenceGoal(int len, const int8_t* sequence);

  const char* typeDescription() { return "Create Sequence"; }
  void draw();
  bool isAchieved(Computer& computer);
};

//--------------------------------------------------------------------------------------------------

#define TURN 0
#define DATA 128

const uint8_t fixedCountTo12[4] = { 6|TURN, 10|TURN, 52|TURN, 54|TURN };
const uint8_t fixedLadder[4] = { 0|DATA, 1|DATA, 2|TURN, 9|TURN };
const uint8_t fixedExit4[9] = {
  3|TURN, 17|TURN, 24|TURN, 29|TURN, 41|TURN, 45|TURN, 55|TURN, 70|TURN, 76|TURN
};
const uint8_t fixedFogBank[9] = {
  54|DATA, 55|DATA, 56|DATA, 57|DATA, 58|DATA, 59|DATA, 60|DATA, 61|DATA, 62|DATA
};
const uint8_t fixedDottedLine[9] = {
  54|TURN, 55|DATA, 56|TURN, 57|DATA, 58|TURN, 59|DATA, 60|TURN, 61|DATA, 62|TURN
};
const uint8_t fixedSevenAteNine[4] = {
  7|TURN, 9|TURN, 71|TURN, 78|TURN
};
const uint8_t fixedRun100[6] = { 7|TURN, 10|TURN, 44|TURN, 45|TURN, 71|TURN, 74|TURN };

const int8_t sequenceLadder[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
const int8_t sequenceSevenAteNine[7] = { 8, 0, 0, 0, 0, 0, 7 };
const int8_t sequenceOneToFive[5] = { 1, 2, 3, 4, 5 };

const ChallengeSpec challengeSpecs[numChallenges] = {
  {
    .name = "Count to 8",
    .goal = new OutputValueGoal(8, Comparison::Equals),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 0,
    .numData = 8
  },{
    .name = "Count to 12",
    .goal = new OutputValueGoal(12, Comparison::Equals),
    .numFixed = 4,
    .fixed = fixedCountTo12,
    .numTurn = 0,
    .numData = 12
  },{
    .name = "Exit 1",
    .goal = new ExitGoal(4, 9),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 4,
    .numData = 1
  },{
    .name = "Exit 2",
    .goal = new ExitGoal(1, 9),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 6,
    .numData = 1
  },{
    .name = "Ladder",
    .goal = new SequenceGoal(8, sequenceLadder),
    .numFixed = 4,
    .fixed = fixedLadder,
    .numTurn = 13,
    .numData = 13
  },{
    .name = "Exit 3",
    .goal = new ExitGoal(1, 9),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 2,
    .numData = 2
  },{
    .name = "Exit 4",
    .goal = new ExitGoal(2, 9),
    .numFixed = 9,
    .fixed = fixedExit4,
    .numTurn = 0,
    .numData = 6
  },{
    .name = "Count to 16",
    .goal = new OutputValueGoal(16, Comparison::Equals),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 8,
    .numData = 16
  },{
    .name = "Countdown to -8",
    .goal = new OutputValueGoal(-8, Comparison::Equals),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 8,
    .numData = 10
  },{
    .name = "Fog bank",
    .goal = new ExitGoal(-1, 7),
    .numFixed = 9,
    .fixed = fixedFogBank,
    .numTurn = 5,
    .numData = 1
  },{
    .name = "Dotted line",
    .goal = new ExitGoal(-1, 7),
    .numFixed = 9,
    .fixed = fixedDottedLine,
    .numTurn = 6,
    .numData = 2
  },{
    .name = "One to Five",
    .goal = new SequenceGoal(5, sequenceOneToFive),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 99,
    .numData = 99
  },{
    .name = "Seven Ate Nine",
    .goal = new SequenceGoal(7, sequenceSevenAteNine),
    .numFixed = 4,
    .fixed = fixedSevenAteNine,
    .numTurn = 0,
    .numData = 26
  },{
    .name = "Busy Beaver 100",
    .goal = new RunLengthGoal(100, Comparison::GreaterThan),
    .numFixed = 6,
    .fixed = fixedRun100,
    .numTurn = 0,
    .numData = 6
  },{
    .name = "Count to 32",
    .goal = new OutputValueGoal(32, Comparison::Equals),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 99,
    .numData = 99
  },{
    .name = "Busy Beaver 1000",
    .goal = new RunLengthGoal(1000, Comparison::GreaterThan),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 99,
    .numData = 99
  },{
    .name = "Busy Beaver 10k",
    .goal = new RunLengthGoal(10000, Comparison::GreaterThan),
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 99,
    .numData = 99
  }
};

const Challenge challenges[numChallenges] = {
  Challenge(challengeSpecs[ 0]),
  Challenge(challengeSpecs[ 1]),
  Challenge(challengeSpecs[ 2]),
  Challenge(challengeSpecs[ 3]),
  Challenge(challengeSpecs[ 4]),
  Challenge(challengeSpecs[ 5]),
  Challenge(challengeSpecs[ 6]),
  Challenge(challengeSpecs[ 7]),
  Challenge(challengeSpecs[ 8]),
  Challenge(challengeSpecs[ 9]),
  Challenge(challengeSpecs[10]),
  Challenge(challengeSpecs[11]),
  Challenge(challengeSpecs[12]),
  Challenge(challengeSpecs[13]),
  Challenge(challengeSpecs[14]),
  Challenge(challengeSpecs[15]),
  Challenge(challengeSpecs[16]),
};

//--------------------------------------------------------------------------------------------------
// ExitGoal

ExitGoal::ExitGoal(int exitX, int exitY) {
  _exitX = exitX;
  _exitY = exitY;
}

void ExitGoal::draw() {
  int x = getDisplayX(_exitX);
  int y = getDisplayY(_exitY);

  // Draw sign
  gb.display.setColor(GREEN);
  gb.display.fillRect(x - 1, y - 1, 7, 7);

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

bool ExitGoal::isAchieved(Computer& computer) {
  ProgramPointer pp = computer.getProgramPointer();

  return (pp.x == _exitX && pp.y == _exitY);
}

//--------------------------------------------------------------------------------------------------
// ComparisonBasedGoal

ComparisonBasedGoal::ComparisonBasedGoal(char targetVar, int targetVal, Comparison comparison) {
  _targetVar = targetVar;
  _targetVal = targetVal;
  _comparison = comparison;
}

void ComparisonBasedGoal::draw() {
  gb.display.setCursor(0, 0);
  gb.display.printf("%c%c%d", _targetVar, comparisonChars[(int)_comparison], _targetVal);
}

bool ComparisonBasedGoal::isAchieved(int value) {
  switch (_comparison) {
    case Comparison::Equals:
      return value == _targetVal;
    case Comparison::GreaterThan:
      return value > _targetVal;
    case Comparison::LessThan:
      return value < _targetVal;
    default:
      return false;
  }
}

//--------------------------------------------------------------------------------------------------
// OutputValueGoal

OutputValueGoal::OutputValueGoal(int target, Comparison comparison)
  : ComparisonBasedGoal('V', target, comparison) {}

void OutputValueGoal::draw() {
  gb.display.setColor(LIGHTGREEN);
  ComparisonBasedGoal::draw();
}

bool OutputValueGoal::isAchieved(Computer& computer) {
  int dp = computer.getDataPointer();

  if (!computer.isDataAddressValid(dp)) {
    return false;
  }
  else {
    return ComparisonBasedGoal::isAchieved(computer.getData(dp));
  }
}

//--------------------------------------------------------------------------------------------------
// RunLengthGoal

RunLengthGoal::RunLengthGoal(int target, Comparison comparison)
  : ComparisonBasedGoal('R', target, comparison) {}

void RunLengthGoal::draw() {
  gb.display.setColor(BLUE);
  ComparisonBasedGoal::draw();
}

bool RunLengthGoal::isAchieved(Computer& computer) {
  return ComparisonBasedGoal::isAchieved(computer.getNumSteps());
}

//--------------------------------------------------------------------------------------------------
// SequenceGoal

SequenceGoal::SequenceGoal(int len, const int8_t* sequence) {
  _len = len;
  _sequence = sequence;
}

int SequenceGoal::drawDots(int x) {
  for (int i = 0; i < 2; i++) {
    gb.display.drawPixel(x + i * 2, 4);
  }
  return 4;
}

int SequenceGoal::drawNumber(int num, int x) {
  gb.display.setCursor(x, 0);
  return gb.display.print(num, DEC) * 4;
}

void SequenceGoal::draw() {
  gb.display.setColor(GREEN);
  gb.display.setCursor(0,0);
  gb.display.print("S=");

  int x = 9;
  x += drawDots(x) + 1;
  x += drawNumber(0, x) + 1;
  for (int i = 0; i < _len; i++) {
    x += drawNumber(_sequence[i], x) + 1;
  }
  x += drawNumber(0, x) + 1;
  drawDots(x);
}

bool SequenceGoal::isAchieved(Computer& computer) {
  int p = 0, q = 0;

  // Find first non-zero cell
  while (p < dataSize && computer.getData(p) == 0) {
    p++;
  }

  // Try to match with sequence
  while (p < dataSize && q < _len && computer.getData(p) == _sequence[q]) {
    p++;
    q++;
  }

  // Check remaining zeros
  while (p < dataSize && computer.getData(p) == 0) {
    p++;
  }

  return (p == dataSize) && (q == _len);
}

//--------------------------------------------------------------------------------------------------
// Challenge

Challenge::Challenge(const ChallengeSpec& spec) : _spec(spec) {}

const char* Challenge::typeDescription() const {
  return _spec.goal->typeDescription();
}

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

