#include "EditController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Computer.h"
#include "Drawing.h"
#include "Store.h"
#include "Challenges.h"

const int maxEditMenuEntries = 5;
const char* availableEditMenuEntries[maxEditMenuEntries] = {
  "Run",
  "Store",
  "Load",
  "Clear",
  "Back to main menu"
};
const char* editMenuEntries[maxEditMenuEntries];

int populateEditMenu() {
  int entry = 0;
  editMenuEntries[entry++] = availableEditMenuEntries[0];

  if (activeChallenge == NO_CHALLENGE) {
    // Only support Store and Load in Experiment mode
    editMenuEntries[entry++] = availableEditMenuEntries[1];
    editMenuEntries[entry++] = availableEditMenuEntries[2];
  }

  editMenuEntries[entry++] = availableEditMenuEntries[3];
  editMenuEntries[entry++] = availableEditMenuEntries[4];

  return entry; // Returns number of entries
}

void editMenu() {
  // Populate menu with entries, which may vary depending on state
  int numEntries = populateEditMenu();
  int menuOption = gb.gui.menu("Edit menu", editMenuEntries, numEntries);

  // Map selected option to index in availableEditMenuEntries array
  const char* selectedEntry = editMenuEntries[menuOption];
  int entry = 0;
  while (strncmp(selectedEntry, availableEditMenuEntries[entry], 20) != 0) {
    entry++;
  }

  switch (entry) {
    case 0:
      setController(&runController);
      break;
    case 1:
      storeProgram(computer);
      break;
    case 2:
      loadProgram(computer);
      break;
    case 3:
      computer.clear();
      break;
    case 4:
      setController(&mainMenuController);
      break;
  }
}

void EditController::trySetInstruction(Instruction instruction) {
  if (
    activeChallenge == NO_CHALLENGE ||
    !challenges[activeChallenge].isFixed(_x, _y)
  ) {
    // Only set the instruction if the challenge, if any, allows it
    computer.setInstruction(_x, _y, instruction);
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
    trySetInstruction(
      (Instruction)(((int)computer.getInstruction(_x, _y) + 1) % 3)
    );
  }
  else if (gb.buttons.pressed(BUTTON_B)) {
    trySetInstruction(Instruction::Noop);
  }
}

void EditController::draw() {
  drawProgramSpace();
  drawProgram(computer);
  drawCursor(_x, _y);

  if (activeChallenge != NO_CHALLENGE) {
    challenges[activeChallenge].draw();
  }
}

