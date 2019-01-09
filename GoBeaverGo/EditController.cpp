/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

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
  "Save",
  "Load",
  "Clear",
  "Back to main menu"
};
const char* editMenuEntries[maxEditMenuEntries];

int populateEditMenu() {
  int entry = 0;
  editMenuEntries[entry++] = availableEditMenuEntries[0];

  if (activeChallenge == nullptr) {
    // Only enable Save and Load when experimenting
    editMenuEntries[entry++] = availableEditMenuEntries[1];
    editMenuEntries[entry++] = availableEditMenuEntries[2];
  }

  editMenuEntries[entry++] = availableEditMenuEntries[3];
  editMenuEntries[entry++] = availableEditMenuEntries[4];

  return entry; // Returns number of entries
}

void EditController::editMenu() {
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
      saveProgram(computer);
      break;
    case 2:
      loadProgram(computer);
      break;
    case 3:
      reset();
      break;
    case 4:
      setController(&mainMenuController);
      break;
  }
}

// Returns the next available instruction for the current position
Instruction EditController::nextAvailableInstruction() {
  int origIndex = (int)computer.getInstruction(_x, _y);

  // Iterate over all instructions that are not NOOP, starting at the next instruction.
  // This way, if the current instruction is the only option, it is still returned.
  int cnt = 0;
  do {
    int instructionIndex = (1 + (origIndex + cnt) % (numInstructions - 1));
    if (_numAvailable[instructionIndex] > 0 || instructionIndex == origIndex) {
      return (Instruction)instructionIndex;
    }
  } while (++cnt < numInstructions);

  return Instruction::Noop;
}

void EditController::trySetInstruction(Instruction instruction) {
  if (instruction == computer.getInstruction(_x, _y)) {
    return; // Ignore, no change
  }

  if (activeChallenge == nullptr || !activeChallenge->isFixed(_x, _y, computer.getSize())) {
    // Only set the instruction if the challenge, if any, allows it
    _numAvailable[ (int)computer.getInstruction(_x, _y) ]++;
    computer.setInstruction(_x, _y, instruction);
    _numAvailable[ (int)computer.getInstruction(_x, _y) ]--;
  } else {
    gb.sound.playTick();
  }
}

void EditController::drawAvailable(Instruction instruction, int y) {
  int num = _numAvailable[ (int)instruction ];

  if (num > 0) {
    gb.display.setColor(DARKBLUE);
    gb.display.fillRect(64, y, 16, 9);

    drawInstruction(66, y + 3, instruction);

    gb.display.setCursor(71, y + 2);
    gb.display.printf("%2d", num);
  }
}

void EditController::reset() {
  _x = 0;
  _y = 0;
  computer.clear();
  _numAvailable[0] = 99; // Always "infinite" NOOPs

  if (activeChallenge == nullptr) {
    for (int i = 1; i < numInstructions; i++) {
      _numAvailable[i] = 99;
    }
    return;
  }

  activeChallenge->setFixedInstructions(computer);
  for (int i = 1; i < numInstructions; i++) {
    _numAvailable[i] = activeChallenge->numAvailable( (Instruction)i );
  }
}

void EditController::update() {
  if (gb.buttons.pressed(BUTTON_MENU)) {
    editMenu();
    return;
  }

  if (gb.buttons.pressed(BUTTON_LEFT)) {
    _x = (_x + computer.getSize() - 1) % computer.getSize();
  }
  else if (gb.buttons.pressed(BUTTON_RIGHT)) {
    _x = (_x + 1) % computer.getSize();
  }
  else if (gb.buttons.pressed(BUTTON_DOWN)) {
    _y = (_y + computer.getSize() - 1) % computer.getSize();
  }
  else if (gb.buttons.pressed(BUTTON_UP)) {
    _y = (_y + 1) % computer.getSize();
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

  if (activeChallenge != nullptr) {
    activeChallenge->draw();

    drawAvailable(Instruction::Turn, 9);
    drawAvailable(Instruction::Data, _numAvailable[1] ? 17 : 9);
  }
}

