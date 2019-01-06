#ifndef __EDIT_CONTROLLER_INCLUDED
#define __EDIT_CONTROLLER_INCLUDED

#include "Controller.h"
#include "Computer.h"

class EditController : public Controller {
  int _x = 0;
  int _y = 0;
  int _numAvailable[numInstructions];

  Instruction nextAvailableInstruction();
  void trySetInstruction(Instruction instruction);

  void drawAvailable(Instruction instruction, int y);

public:
  void update();
  void draw();

  // Resets the controller
  void reset();
};

#endif
