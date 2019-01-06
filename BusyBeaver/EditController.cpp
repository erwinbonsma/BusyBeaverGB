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

// Returns the next available instruction for the current position
Instruction EditController::nextAvailableInstruction() {
  int i = (int)computer.getInstruction(_x, _y);

  // Iterate over all instructions that are not NOOP, starting at the next instruction.
  // This way, if the current instruction is the only option, it is still returned.
  int cnt = 0;
  do {
    int instructionIndex = (1 + (i + cnt) % (numInstructions - 1));
    if (_numAvailable[instructionIndex] > 0) {
      return (Instruction)instructionIndex;
    }
  } while (++cnt < numInstructions);

  return Instruction::Noop;
}

void EditController::trySetInstruction(Instruction instruction) {
  if (instruction == computer.getInstruction(_x, _y)) {
    return; // Ignore, no change
  }

  if (
    activeChallenge == NO_CHALLENGE ||
    !challenges[activeChallenge].isFixed(_x, _y)
  ) {
    // Only set the instruction if the challenge, if any, allows it
    _numAvailable[ (int)computer.getInstruction(_x, _y) ]++;
    computer.setInstruction(_x, _y, instruction);
    _numAvailable[ (int)computer.getInstruction(_x, _y) ]--;
  }
}

void EditController::drawAvailable(Instruction instruction, Color color, int y) {
  int num = _numAvailable[ (int)instruction ];

  if (num > 0) {
    gb.display.setColor(BLUE);
    gb.display.fillRect(66, y, 14, 7);

    gb.display.setColor(color);
    gb.display.fillRect(67, y + 2, 3, 3);

    gb.display.setCursor(72, y + 1);
    gb.display.printf("%2d", num);
  }
}

void EditController::reset() {
  _x = 0;
  _y = 0;
  computer.clear();
  _numAvailable[0] = 99; // Always "infinite" NOOPs

  if (activeChallenge == NO_CHALLENGE) {
    for (int i = 1; i < numInstructions; i++) {
      _numAvailable[i] = 99;
    }
    return;
  }

  const Challenge& challenge = challenges[activeChallenge];

  challenge.setFixedInstructions(computer);
  for (int i = 1; i < numInstructions; i++) {
    _numAvailable[i] = challenge.numAvailable( (Instruction)i );
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
    trySetInstruction(nextAvailableInstruction());
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

    drawAvailable(Instruction::Turn, BLACK, 8);
    drawAvailable(Instruction::Data, WHITE, _numAvailable[1] ? 15 : 8);
  }
}

