/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Computer.h"

class ChallengeGoal;

struct ChallengeSpec {
  const char* name;
  ChallengeGoal* goal;

  uint8_t numFixed;
  const uint8_t* fixed;

  uint8_t numTurn;
  uint8_t numData;
};


class Challenge {
  const ChallengeSpec& _spec;

public:
  Challenge(const ChallengeSpec& spec);

  const char* name() const { return _spec.name; }
  const char* typeDescription() const;

  void setFixedInstructions(Computer& computer) const;
  bool isFixed(int x, int y) const;

  int numAvailable(Instruction instruction) const;

  void draw() const;
  bool isAchieved(Computer& computer) const;
};

const int numChallenges = 17;
extern const Challenge challenges[numChallenges];

#define NO_CHALLENGE  -1
extern int activeChallenge;

