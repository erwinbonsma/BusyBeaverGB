/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "RunController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Computer.h"
#include "Drawing.h"
#include "Challenges.h"
#include "Store.h"

const uint8_t buttonData[] = {
  7, 7, 4, 0, 1, 0xFF, 1,
  // Play
  0x00, 0x10, 0x00, 0x00,
  0x00, 0x11, 0x00, 0x00,
  0x00, 0x11, 0x10, 0x00,
  0x00, 0x11, 0x11, 0x00,
  0x00, 0x11, 0x10, 0x00,
  0x00, 0x11, 0x00, 0x00,
  0x00, 0x10, 0x00, 0x00,

  // Pause
  0x01, 0x10, 0x11, 0x00,
  0x01, 0x10, 0x11, 0x00,
  0x01, 0x10, 0x11, 0x00,
  0x01, 0x10, 0x11, 0x00,
  0x01, 0x10, 0x11, 0x00,
  0x01, 0x10, 0x11, 0x00,
  0x01, 0x10, 0x11, 0x00,

  // Step
  0x11, 0x01, 0x00, 0x00,
  0x11, 0x01, 0x10, 0x00,
  0x11, 0x01, 0x11, 0x00,
  0x11, 0x01, 0x11, 0x10,
  0x11, 0x01, 0x11, 0x00,
  0x11, 0x01, 0x10, 0x00,
  0x11, 0x01, 0x00, 0x00,

  // Rewind
  0x00, 0x00, 0x00, 0x10,
  0x00, 0x00, 0x01, 0x10,
  0x11, 0x00, 0x11, 0x10,
  0x11, 0x01, 0x11, 0x10,
  0x11, 0x00, 0x11, 0x10,
  0x00, 0x00, 0x01, 0x10,
  0x00, 0x00, 0x00, 0x10
};

Image buttons(buttonData);

constexpr int numRunMenuEntries = 4;
char const* const runMenuEntries[numRunMenuEntries] = {
  "Edit",
  "Reset",
  "Back to main menu",
  "Cancel"
};

void nextChallenge() {
  activeChallenge = activeChallengeSet->nextChallenge(activeChallenge);
  if (activeChallenge != nullptr) {
    // Go to next challenge
    setController(&introController);
  } else {
    // No more challenges
    setController(&mainMenuController);
  }
}

void RunController::runMenu() {
  int entry = gb.gui.menu("Run menu", (const char**)runMenuEntries, numRunMenuEntries);

  switch (entry) {
    case 0:
      setController(&editController);
      break;
    case 1:
      computer.reset();
      _paused = true;
      break;
    case 2:
      setController(&mainMenuController);
      break;
    case 3:
      break;
  }
}

RunAction RunController::activeActionButtonA() {
  switch (computer.getStatus()) {
    case Status::Done:
    case Status::Error:
      return RunAction::Rewind;

    case Status::Ready:
    case Status::Running:
      if (_runSpeed == 0) {
        return RunAction::Step;
      } else {
        return _paused ? RunAction::Play : RunAction::Pause;
      }
  }

  return RunAction::None;
}

void RunController::changeRunSpeed(int delta) {
  setRunSpeed( min(maxRunSpeed, max(0, _runSpeed + delta)) );
}

void RunController::setRunSpeed(int speed) {
  _runSpeed = speed;

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

void RunController::tryChangeTapeShift(int delta) {
  if (computer.getStatus() != Status::Running || _paused || _runSpeed == 0) {
    // Only allow moving the tape when the program is not actively running
    _tapeShift += delta;
  }
}

void RunController::decayLedActivation(uint8_t amount) {
  _ledActivation.inc -= min(_ledActivation.inc, amount);
  _ledActivation.dec -= min(_ledActivation.dec, amount);
  _ledActivation.shr -= min(_ledActivation.shr, amount);
  _ledActivation.shl -= min(_ledActivation.shl, amount);
}

void RunController::updateLedActivation() {
  ExecutionStats stats = computer.getExecutionStats();

  if (stats.inc != stats.dec) {
    if (stats.inc > stats.dec) {
      _ledActivation.inc = max(
        (uint8_t)((uint32_t)255 * (stats.inc - stats.dec) / (stats.inc + stats.dec)),
        _ledActivation.inc
      );
    } else {
      _ledActivation.dec = max(
        (uint8_t)((uint32_t)255 * (stats.dec - stats.inc) / (stats.inc + stats.dec)),
        _ledActivation.dec
      );
    }
  }
  if (stats.shr != stats.shl) {
    if (stats.shr > stats.shl) {
      _ledActivation.shr = max(
        (uint8_t)((uint32_t)255 * (stats.shr - stats.shl) / (stats.shr + stats.shl)),
        _ledActivation.shr
      );
    } else {
      _ledActivation.shl = max(
        (uint8_t)((uint32_t)255 * (stats.shl - stats.shr) / (stats.shr + stats.shl)),
        _ledActivation.shl
      );
    }
  }

  computer.resetExecutionStats();
}

void RunController::drawLeds() {
  gb.lights.setColor(gb.createColor(_ledActivation.inc, 0, 0));
  gb.lights.drawPixel(0, 0);
  gb.lights.drawPixel(1, 0);

  gb.lights.setColor(gb.createColor(0, 0, _ledActivation.dec));
  gb.lights.drawPixel(0, 3);
  gb.lights.drawPixel(1, 3);

  gb.lights.setColor(gb.createColor(0, _ledActivation.shl, 0));
  gb.lights.drawPixel(0, 1);
  gb.lights.drawPixel(0, 2);

  gb.lights.setColor(gb.createColor(0, _ledActivation.shr, 0));
  gb.lights.drawPixel(1, 1);
  gb.lights.drawPixel(1, 2);
}


char popupBuf[20];

void RunController::handleProgramTermination() {
  if (computer.getStatus() == Status::Error) {
    return;
  }

  if (activeChallenge != nullptr) {
    if (activeChallenge->isAchieved(computer)) {
      recordChallengeCompleted(
        activeChallengeSet->indexOfChallenge(activeChallenge),
        activeChallengeSet == &tutorialsSet
      );
      _challengeCompleted = true;

      snprintf(popupBuf, sizeof(popupBuf), "%s done!", activeChallengeSet->challengeType());
    } else {
      snprintf(popupBuf, sizeof(popupBuf), "%s failed", activeChallengeSet->challengeType());
    }
    gb.gui.popup(popupBuf, 40);
  } else {
    recordExperimentDone(computer);
  }
}


RunController::RunController() {
  setRunSpeed(4); // Set default speed
}

void RunController::reset() {
  computer.reset();
  _paused = false;
  _challengeCompleted = false;
  _tapeShift = 0;

  resetDrawing();
}

void RunController::activate() {
  reset();
}

void RunController::update() {
  decayLedActivation(8);

  if (gb.buttons.pressed(BUTTON_MENU)) {
    runMenu();
    return;
  }

  if (gb.buttons.repeat(BUTTON_UP, 4)) {
    changeRunSpeed(+1);
    _tapeShift = 0;
  }
  else if (gb.buttons.repeat(BUTTON_DOWN, 4)) {
    changeRunSpeed(-1);
  }
  else if (gb.buttons.repeat(BUTTON_RIGHT, 4)) {
    tryChangeTapeShift(+1);
  }
  else if (gb.buttons.repeat(BUTTON_LEFT, 4)) {
    tryChangeTapeShift(-1);
  }
  else if (gb.buttons.released(BUTTON_A)) {
    switch (activeActionButtonA()) {
      case RunAction::Play:
        _paused = false;
        _tapeShift = 0;
        break;
      case RunAction::Pause:
        _paused = true;
        break;
      case RunAction::Rewind:
        reset();
        _paused = true;
        break;
      case RunAction::Step:
        computer.step();
        updateLedActivation();
        _tapeShift = 0;
        _ticksSinceLastStep = 0;
        break;
      default:
        // void
        break;
    }
  }
  else if (gb.buttons.released(BUTTON_B)) {
    setController(&editController);
  }

  bool hasTerminated = computer.hasTerminated();
  if (!hasTerminated) {
    if (_runSpeed > 0 && !_paused) {
      if (++_ticksSinceLastStep >= _stepPeriod) {
        for (int i = 0; i < _stepsPerTick; i++) {
          computer.step();
        }
        updateLedActivation();
        _ticksSinceLastStep = 0;
      }
    }
  } else if (
    _challengeCompleted
    && activeChallenge != challengesSet.lastChallenge()
    && ++_ticksSinceLastStep > 50
  ) {
    nextChallenge();
  }

  if (!hasTerminated && computer.hasTerminated()) {
    // Program just terminated
    handleProgramTermination();
  }
}

void RunController::draw() {
  drawProgramSpace();
  drawVisitCounts(computer);
  drawProgram(computer);

  drawData(computer, computer.getDataPointer() + _tapeShift);

  if (activeChallenge != nullptr) {
    activeChallenge->draw();
  }

  // Draw after challenge (as pointer may be above exit)
  drawProgramPointer(computer);

  drawRunStatus(computer);

  drawSpeedBar(_runSpeed);

//  gb.display.setColor(WHITE);
//  gb.display.setCursor(0, 52);
//  gb.display.printf("%3d/%3d %3d/%3d",
//    _ledActivation.shl, _ledActivation.shr,
//    _ledActivation.dec, _ledActivation.inc
//  );

  buttons.setFrame((int)activeActionButtonA());
  gb.display.drawImage(73, 33, buttons);

  drawLeds();
}
