#ifndef __RUN_CONTROLLER_INCLUDED
#define __RUN_CONTROLLER_INCLUDED

#include "Controller.h"

const int maxRunSpeed = 20;

class RunController : public Controller {
  int _runSpeed = 0;
  int _stepPeriod = 0;
  int _stepsPerTick = 0;
  int _ticksSinceLastStep = 0;

  void changeRunSpeed(int delta);

public:
  void update();
  void draw();
};

#endif
