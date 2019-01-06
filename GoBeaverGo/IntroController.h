#ifndef __INTRO_CONTROLLER_INCLUDED
#define __INTRO_CONTROLLER_INCLUDED

#include "Controller.h"
#include "Computer.h"

class IntroController : public Controller {
  int _updateCount;

public:
  void activate();
  void update();
  void draw();
};

#endif
