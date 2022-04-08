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
  else if (gb.buttons.pressed(BUTTON_RIGHT)) {
    tryChangeTapeShift(+1);
  }
  else if (gb.buttons.pressed(BUTTON_LEFT)) {
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
        _ticksSinceLastStep = 0;
      }
    }
  } else {
    if (_challengeCompleted) {
      if (++_ticksSinceLastStep > 50) {
        nextChallenge();
      }
    }
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

  buttons.setFrame((int)activeActionButtonA());
  gb.display.drawImage(73, 33, buttons);
}
