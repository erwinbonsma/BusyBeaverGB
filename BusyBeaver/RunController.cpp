#include "RunController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Program.h"
#include "Drawing.h"

const int unitRunSpeed = 6;

const char* runMenuEntries[] = {
  "Continue",
  "Reset",
  "Edit",
};

void runMenu() {
  int entry = gb.gui.menu("Run menu", runMenuEntries);

  switch (entry) {
    case 0:
      break;
    case 1:
      program.reset();
      break;
    case 2:
      activeController = &editController;
      break;
  }
}

void RunController::changeRunSpeed(int delta) {
  _runSpeed = min(maxRunSpeed, max(0, _runSpeed + delta));

  if (_runSpeed == 0) {
    return;
  }

  if (_runSpeed < unitRunSpeed) {
    _stepPeriod = 1 << (unitRunSpeed - _runSpeed);
    _stepsPerTick = 1;
  } else {
    _stepPeriod = 1;
    _stepsPerTick = 1 << (_runSpeed - unitRunSpeed);
  }
}

void RunController::update() {
  if (gb.buttons.pressed(BUTTON_MENU)) {
    runMenu();
    return;
  }

  if (gb.buttons.pressed(BUTTON_UP)) {
    changeRunSpeed(+1);
  }
  else if (gb.buttons.pressed(BUTTON_DOWN)) {
    changeRunSpeed(-1);
  }
  else if (gb.buttons.pressed(BUTTON_A)) {
    program.step();
  }
  else if (gb.buttons.pressed(BUTTON_B)) {
    program.reset();
  }

  if (_runSpeed > 0) {
    if (++_ticksSinceLastStep >= _stepPeriod) {
      for (int i = 0; i < _stepsPerTick; i++) {
        program.step();
        _ticksSinceLastStep = 0;
      }
    }
  }
}

void RunController::draw() {
  drawProgramSpace();
  drawVisitCounts(program);
  drawProgram(program);
  drawProgramPointer(program);

  drawMemory(program);

  drawRunStatus(program);

  drawSpeedBar(_runSpeed);
}

