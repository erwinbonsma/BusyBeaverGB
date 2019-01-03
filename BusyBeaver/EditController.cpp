#include "EditController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Program.h"
#include "Drawing.h"

const char* editMenuEntries[] = {
  "Continue",
  "Clear",
  "Run",
};

void editMenu() {
  int entry = gb.gui.menu("Edit menu", editMenuEntries);

  switch (entry) {
    case 0:
      break;
    case 1:
      program.clear();
      break;
    case 2:
      activeController = &runController;
      program.reset();
      break;
  }
}

void EditController::update() {
  if (gb.buttons.pressed(BUTTON_MENU)) {
    editMenu();
    return;
  }

  if (gb.buttons.pressed(BUTTON_LEFT)) {
    _x = (_x + maxProgramSize - 1) % maxProgramSize;
  }
  else if (gb.buttons.pressed(BUTTON_RIGHT)) {
    _x = (_x + 1) % maxProgramSize;
  }
  else if (gb.buttons.pressed(BUTTON_DOWN)) {
    _y = (_y + maxProgramSize - 1) % maxProgramSize;
  }
  else if (gb.buttons.pressed(BUTTON_UP)) {
    _y = (_y + 1) % maxProgramSize;
  }
  else if (gb.buttons.pressed(BUTTON_A)) {
    program.setInstruction(
      _x, _y,
      (Instruction)(((int)program.getInstruction(_x, _y) + 1) % 3)
    );
  }
  else if (gb.buttons.pressed(BUTTON_B)) {
    program.setInstruction(_x, _y, Instruction::Noop);
  }
}

void EditController::draw() {
  drawProgramSpace();
  drawProgram(program);
  drawCursor(_x, _y);
}

