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

class RunController : public Controller {
  int _runSpeed = 0;
  int _stepPeriod = 0;
  int _stepsPerTick = 0;
  int _ticksSinceLastStep = 0;
  bool _paused;

  void runMenu();
  RunAction activeActionButtonA();
  void changeRunSpeed(int delta);

public:
  void activate();
  void update();
  void draw();
};

#endif
