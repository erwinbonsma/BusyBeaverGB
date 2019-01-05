#ifndef __EDIT_CONTROLLER_INCLUDED
#define __EDIT_CONTROLLER_INCLUDED

#include "Controller.h"
#include "Computer.h"

class EditController : public Controller {
  int _x = 0;
  int _y = 0;

  void trySetInstruction(Instruction instruction);

public:
  void update();
  void draw();
};

#endif
