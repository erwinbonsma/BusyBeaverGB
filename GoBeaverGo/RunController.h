/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __RUN_CONTROLLER_INCLUDED
#define __RUN_CONTROLLER_INCLUDED

#include "Controller.h"

const int maxRunSpeed = 20;

enum class RunAction : int {
  Play = 0,
  Pause = 1,
  Step = 2,
  Rewind = 3,
  None = 8
};

enum class ChallengeStatus : int {
  None,
  Undecided,
  Completed,
  Failed
};

class RunController : public Controller {
  int _runSpeed = 0;
  int _stepPeriod;
  int _stepsPerTick;
  int _ticksSinceLastStep = 0;
  bool _paused;
  ChallengeStatus _challengeStatus;

  void runMenu();
  RunAction activeActionButtonA();
  void changeRunSpeed(int delta);

  void reset();

public:
  RunController();

  void activate();
  void update();
  void draw();
};

#endif
