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
  const ChallengeGoal* goal;

  uint8_t numFixed;
  const uint8_t* fixed;

  uint8_t numTurn;
  uint8_t numData;
  uint8_t programSize;
};


class Challenge {
  const ChallengeSpec& _spec;

public:
  Challenge(const ChallengeSpec& spec);

  const char* name() const { return _spec.name; }
  const char* typeDescription() const;
  uint8_t programSize() const { return _spec.programSize; }

  void setFixedInstructions(Computer& computer) const;
  bool isFixed(int x, int y, int programSize) const;

  int numAvailable(Instruction instruction) const;

  void draw() const;
  bool isAchieved(Computer& computer) const;
};

class ChallengeSet {
  int _numChallenges;
  const Challenge* _challenges;
  const char* _challengeType;

public:
  ChallengeSet(const char* type, const Challenge* challenges, int num);

  const char* challengeType() const { return _challengeType; }

  int size() const { return _numChallenges; }
  int indexOfChallenge(const Challenge* challenge) const;
  const Challenge* nextChallenge(const Challenge* challenge) const;
  const Challenge* challengeAt(int index) const { return &_challenges[index]; }
  const Challenge* lastChallenge() const { return &_challenges[_numChallenges - 1]; }
};

extern const ChallengeSet tutorialsSet;
extern const ChallengeSet challengesSet;

extern const Challenge* activeChallenge;
extern const ChallengeSet* activeChallengeSet;
