/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __RUN_CONTROLLER_INCLUDED
#define __RUN_CONTROLLER_INCLUDED

#include <Gamebuino-Meta.h>

#include "Controller.h"

constexpr uint8_t unitRunSpeed = 6;
constexpr uint8_t maxRunSpeed = 20;

enum class RunAction : int {
  Play = 0,
  Pause = 1,
  Step = 2,
  Rewind = 3,
  None = 8
};

struct LedActivation {
  uint8_t inc;
  uint8_t dec;
  uint8_t shr;
  uint8_t shl;
};

class RunController : public Controller {
  LedActivation _ledActivation;
  int16_t _tapeShift;
  uint16_t _stepsPerTick;
  uint8_t _runSpeed = 0;
  uint8_t _stepPeriod;
  uint8_t _ticksSinceLastStep = 0;
  bool _paused;
  bool _challengeCompleted;

  void runMenu();
  RunAction activeActionButtonA();
  void changeRunSpeed(int delta);
  void tryChangeTapeShift(int delta);
  void handleProgramTermination();

  void reset();

  void decayLedActivation(uint8_t amount);
  void updateLedActivation();
  void drawLeds();

public:
  RunController();

  void setRunSpeed(int speed);

  void activate();
  void update();
  void draw();
};

#endif
