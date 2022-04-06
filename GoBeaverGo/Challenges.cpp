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

char const comparisonChars[3] = { '=', '>', '<' };

const Challenge* activeChallenge = nullptr;
const ChallengeSet* activeChallengeSet = nullptr;

//--------------------------------------------------------------------------------------------------
// ChallengeGoal definitions

class ChallengeGoal {
public:
  virtual const char* typeDescription() const = 0;
  virtual void draw() const = 0;
  virtual bool isAchieved(Computer& computer) const = 0;
};

class ExitGoal : public ChallengeGoal {
  int _exitX, _exitY;
public:
  ExitGoal(int exitX, int exitY);

  const char* typeDescription() const { return "Reach the Exit"; }
  void draw() const;
  bool isAchieved(Computer& computer) const;
};

class ComparisonBasedGoal : public ChallengeGoal {
  char _targetVar;
  int _targetVal;
  Comparison _comparison;

protected:
  bool isAchieved(int value) const;

public:
  ComparisonBasedGoal(char targetVar, int targetVal, Comparison comparison);

  void draw() const;
};

class OutputValueGoal : public ComparisonBasedGoal {
public:
  OutputValueGoal(int target, Comparison comparison);

  const char* typeDescription() const { return "Output Value"; }
  void draw() const;
  bool isAchieved(Computer& computer) const;
};

class RunLengthGoal : public ComparisonBasedGoal {
public:
  RunLengthGoal(int target, Comparison comparison);

  const char* typeDescription() const { return "Run Length"; }
  void draw() const;
  bool isAchieved(Computer& computer) const;
};

class SequenceGoal : public ChallengeGoal {
  int _len;
  const int8_t* _sequence;

  int drawDots(int x) const;
  int drawNumber(int num, int x) const;

public:
  SequenceGoal(int len, const int8_t* sequence);

  const char* typeDescription() const { return "Create Sequence"; }
  void draw() const;
  bool isAchieved(Computer& computer) const;
};

//--------------------------------------------------------------------------------------------------

#define TURN 0
#define DATA 128

const uint8_t fixedTurnRight[1] = { 20|TURN };
const uint8_t fixedSharpTurn[2] = { 16|TURN, 20|TURN };
const uint8_t fixedShiftRight[4] = { 15|DATA, 19|TURN, 20|TURN, 23|DATA };
const uint8_t* fixedDecrement = fixedShiftRight;
const uint8_t fixedShiftLeft[6] = { 2|DATA, 3|TURN, 6|TURN, 15|DATA, 19|TURN, 20|TURN };

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

const int8_t sequenceTwoOnes[2] = { 1, 1 };
const int8_t sequenceShiftLeft[2] = { -1, 1 };
const int8_t sequenceLadder[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
const int8_t sequenceSevenAteNine[7] = { 8, 0, 0, 0, 0, 0, 7 };
const int8_t sequenceOneToFive[5] = { 1, 2, 3, 4, 5 };

const OutputValueGoal goalOutput0(0, Comparison::Equals);
const OutputValueGoal goalOutput1(1, Comparison::Equals);
const OutputValueGoal goalOutput12(12, Comparison::Equals);
const OutputValueGoal goalOutput16(16, Comparison::Equals);
const OutputValueGoal goalOutputMinus8(-8, Comparison::Equals);
const OutputValueGoal goalOutput32(32, Comparison::Equals);
const ExitGoal goalExit19(1, 9);
const ExitGoal goalExit29(2, 9);
const ExitGoal goalExit49(4, 9);
const ExitGoal goalExit53(5, 3);
const ExitGoal goalExitMinus17(-1, 7);
const ExitGoal goalExitMinus13(-1, 3);
const SequenceGoal goalSequenceLadder(8, sequenceLadder);
const SequenceGoal goalSequenceSevenAteNine(7, sequenceSevenAteNine);
const SequenceGoal goalSequenceOneToFive(5, sequenceOneToFive);
const SequenceGoal goalSequenceTwoOnes(2, sequenceTwoOnes);
const SequenceGoal goalSequenceShiftLeft(2, sequenceShiftLeft);
const RunLengthGoal goalRunLength100(100, Comparison::GreaterThan);
const RunLengthGoal goalRunLength1000(1000, Comparison::GreaterThan);

const int numChallenges = 15;
const ChallengeSpec challengeSpecs[numChallenges] = {
  {
    .name = "Count to 12",
    .goal = &goalOutput12,
    .numFixed = 4,
    .fixed = fixedCountTo12,
    .numTurn = 0,
    .numData = 12
  },{
    .name = "Exit 1",
    .goal = &goalExit49,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 4,
    .numData = 1
  },{
    .name = "Exit 2",
    .goal = &goalExit19,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 6,
    .numData = 1
  },{
    .name = "Ladder",
    .goal = &goalSequenceLadder,
    .numFixed = 4,
    .fixed = fixedLadder,
    .numTurn = 13,
    .numData = 13
  },{
    .name = "Exit 3",
    .goal = &goalExit19,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 2,
    .numData = 2
  },{
    .name = "Exit 4",
    .goal = &goalExit29,
    .numFixed = 9,
    .fixed = fixedExit4,
    .numTurn = 0,
    .numData = 5
  },{
    .name = "Count to 16",
    .goal = &goalOutput16,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 8,
    .numData = 16
  },{
    .name = "Countdown to -8",
    .goal = &goalOutputMinus8,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 8,
    .numData = 10
  },{
    .name = "Seven Ate Nine",
    .goal = &goalSequenceSevenAteNine,
    .numFixed = 4,
    .fixed = fixedSevenAteNine,
    .numTurn = 0,
    .numData = 26
  },{
    .name = "Fog bank",
    .goal = &goalExitMinus17,
    .numFixed = 9,
    .fixed = fixedFogBank,
    .numTurn = 5,
    .numData = 1
  },{
    .name = "One to Five",
    .goal = &goalSequenceOneToFive,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 99,
    .numData = 99
  },{
    .name = "Dotted line",
    .goal = &goalExitMinus17,
    .numFixed = 9,
    .fixed = fixedDottedLine,
    .numTurn = 6,
    .numData = 2
  },{
    .name = "Busy Beaver 100",
    .goal = &goalRunLength100,
    .numFixed = 6,
    .fixed = fixedRun100,
    .numTurn = 0,
    .numData = 6
  },{
    .name = "Count to 32",
    .goal = &goalOutput32,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 99,
    .numData = 99
  },{
    .name = "Busy Beaver 1000",
    .goal = &goalRunLength1000,
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
};

const ChallengeSet challengesSet("Challenge", 9, challenges, numChallenges);

const int numTutorials = 7;
const ChallengeSpec tutorialSpecs[numTutorials] = {
  {
    .name = "Increment",
    .goal = &goalOutput1,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 0,
    .numData = 1
  },{
    .name = "Turn Left",
    .goal = &goalExitMinus13,
    .numFixed = 0,
    .fixed = nullptr,
    .numTurn = 1,
    .numData = 0
  },{
    .name = "Turn Right",
    .goal = &goalExit53,
    .numFixed = 1,
    .fixed = fixedTurnRight,
    .numTurn = 0,
    .numData = 1
  },{
    .name = "Sharp Turn",
    .goal = &goalOutput1,
    .numFixed = 2,
    .fixed = fixedSharpTurn,
    .numTurn = 0,
    .numData = 1
  },{
    .name = "Shift Right",
    .goal = &goalSequenceTwoOnes,
    .numFixed = 4,
    .fixed = fixedShiftRight,
    .numTurn = 0,
    .numData = 1
  },{
    .name = "Decrement",
    .goal = &goalOutput0,
    .numFixed = 4,
    .fixed = fixedDecrement,
    .numTurn = 0,
    .numData = 1
  },{
    .name = "Shift Left",
    .goal = &goalSequenceShiftLeft,
    .numFixed = 6,
    .fixed = fixedShiftLeft,
    .numTurn = 0,
    .numData = 1
  }
};

const Challenge tutorials[numTutorials] = {
  Challenge(tutorialSpecs[ 0]),
  Challenge(tutorialSpecs[ 1]),
  Challenge(tutorialSpecs[ 2]),
  Challenge(tutorialSpecs[ 3]),
  Challenge(tutorialSpecs[ 4]),
  Challenge(tutorialSpecs[ 5]),
  Challenge(tutorialSpecs[ 6]),
};

const ChallengeSet tutorialsSet("Tutorial", 5, tutorials, numTutorials);


//--------------------------------------------------------------------------------------------------
// ExitGoal

ExitGoal::ExitGoal(int exitX, int exitY) {
  _exitX = exitX;
  _exitY = exitY;
}

void ExitGoal::draw() const {
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
  else if (_exitX > _exitY) {
    gb.display.drawLine(x + 3, y + 1, x + 3, y + 3);
  }
  else {
    gb.display.drawLine(x + 1, y + 1, x + 3, y + 1);
  }
}

bool ExitGoal::isAchieved(Computer& computer) const {
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

void ComparisonBasedGoal::draw() const {
  gb.display.setCursor(0, 0);
  gb.display.printf("%c%c%d", _targetVar, comparisonChars[(int)_comparison], _targetVal);
}

bool ComparisonBasedGoal::isAchieved(int value) const {
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
  : ComparisonBasedGoal('X', target, comparison) {}

void OutputValueGoal::draw() const {
  gb.display.setColor(LIGHTGREEN);
  ComparisonBasedGoal::draw();
}

bool OutputValueGoal::isAchieved(Computer& computer) const {
  return ComparisonBasedGoal::isAchieved(computer.getOutput());
}

//--------------------------------------------------------------------------------------------------
// RunLengthGoal

RunLengthGoal::RunLengthGoal(int target, Comparison comparison)
  : ComparisonBasedGoal('R', target, comparison) {}

void RunLengthGoal::draw() const {
  gb.display.setColor(BLUE);
  ComparisonBasedGoal::draw();
}

bool RunLengthGoal::isAchieved(Computer& computer) const {
  return ComparisonBasedGoal::isAchieved(computer.getNumSteps());
}

//--------------------------------------------------------------------------------------------------
// SequenceGoal

SequenceGoal::SequenceGoal(int len, const int8_t* sequence) {
  _len = len;
  _sequence = sequence;
}

int SequenceGoal::drawDots(int x) const {
  for (int i = 0; i < 2; i++) {
    gb.display.drawPixel(x + i * 2, 4);
  }
  return 4;
}

int SequenceGoal::drawNumber(int num, int x) const {
  gb.display.setCursor(x, 0);
  return gb.display.print(num, DEC) * 4;
}

void SequenceGoal::draw() const {
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

bool SequenceGoal::isAchieved(Computer& computer) const {
  int p = computer.getMinDataAddress(), q = 0;

  // Find first non-zero cell
  while (p <= computer.getMaxDataAddress() && computer.getData(p) == 0) {
    p++;
  }

  // Try to match with sequence
  while (p <= computer.getMaxDataAddress() && q < _len && computer.getData(p) == _sequence[q]) {
    p++;
    q++;
  }

  // Check remaining zeros
  while (p <= computer.getMaxDataAddress() && computer.getData(p) == 0) {
    p++;
  }

  return (p > computer.getMaxDataAddress()) && (q == _len);
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
    int fixX = insCode % computer.getSize();
    int fixY = (insCode - fixX) / computer.getSize();
    computer.setInstruction(fixX, fixY, instruction);
  }
}

bool Challenge::isFixed(int x, int y, int programSize) const {
  for (int i = 0; i < _spec.numFixed; i++) {
    int insCode = (_spec.fixed[i] & 127);
    int fixX = insCode % programSize;
    int fixY = (insCode - fixX) / programSize;
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

//--------------------------------------------------------------------------------------------------
// ChallengeSet implementation

ChallengeSet::ChallengeSet(const char* type, int programSize, const Challenge* challenges, int num) {
  _challengeType = type;
  _programSize = programSize;
  _challenges = challenges;
  _numChallenges = num;
}

int ChallengeSet::indexOfChallenge(const Challenge* challenge) const {
  int i = 0;
  do {
    if (&_challenges[i] == challenge) {
      return i;
    }
  } while (++i < _numChallenges);

  return -1;
}

const Challenge* ChallengeSet::nextChallenge(const Challenge* challenge) const {
  int index = indexOfChallenge(challenge) + 1;
  return (index < _numChallenges) ? &_challenges[index] : nullptr;
}
