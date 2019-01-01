#ifndef __EDIT_CONTROLLER_INCLUDED
#define __EDIT_CONTROLLER_INCLUDED

#include "Controller.h"

class EditController : public Controller {
  int _x = 0;
  int _y = 0;

public:
  void update();
  void draw();
};

#endif
