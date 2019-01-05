#include <Gamebuino-Meta.h>

class Computer;
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

  void setFixedInstructions(Computer& computer) const;
  bool isFixed(int x, int y) const;

  void draw() const;
  bool isAchieved(Computer& computer) const;
};

const int numChallenges = 4;
extern const Challenge challenges[numChallenges];

#define NO_CHALLENGE  -1
extern int activeChallenge;

