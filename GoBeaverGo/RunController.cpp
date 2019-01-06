#include "RunController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Computer.h"
#include "Drawing.h"
#include "Challenges.h"
#include "Store.h"

const int unitRunSpeed = 6;

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

const char* runMenuEntries[] = {
  "Edit",
  "Reset",
  "Back to main menu"
};

void RunController::runMenu() {
  int entry = gb.gui.menu("Run menu", runMenuEntries);

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

RunController::RunController() {
  changeRunSpeed(4); // Set default speed
}

void RunController::reset() {
  computer.reset();
  _paused = true;
  _challengeStatus = (
    activeChallenge != NO_CHALLENGE ?
    ChallengeStatus::Undecided :
    ChallengeStatus::None
  );
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
  else if (gb.buttons.pressed(BUTTON_A)) {
    switch (activeActionButtonA()) {
      case RunAction::Play:
        _paused = false;
        break;
      case RunAction::Pause:
        _paused = true;
        break;
      case RunAction::Rewind:
        reset();
        break;
      case RunAction::Step:
        computer.step();
        break;
      default:
        // void
        break;
    }
  }

  if (_runSpeed > 0 && !_paused) {
    if (++_ticksSinceLastStep >= _stepPeriod) {
      for (int i = 0; i < _stepsPerTick; i++) {
        computer.step();
        _ticksSinceLastStep = 0;
      }
    }
  }

  if (
    computer.getStatus() == Status::Done &&
    _challengeStatus == ChallengeStatus::Undecided
  ) {
    if (challenges[activeChallenge].isAchieved(computer)) {
      _challengeStatus = ChallengeStatus::Completed;
      gb.gui.popup("Challenge done!", 40);

      // Record progress
      setMaxCompletedChallenge(max(activeChallenge + 1, getMaxCompletedChallenge()));

      if (activeChallenge + 1 < numChallenges) {
        // Go to next challenge
        activeChallenge++;
        setController(&introController);
      } else {
        // No more challenges
        setController(&mainMenuController);
      }
    } else {
      _challengeStatus = ChallengeStatus::Failed;
      gb.gui.popup("Challenge failed", 40);
    }
  }
}

void RunController::draw() {
  drawProgramSpace();
  drawVisitCounts(computer);
  drawProgram(computer);
  drawProgramPointer(computer);

  drawData(computer);

  if (activeChallenge != NO_CHALLENGE) {
    challenges[activeChallenge].draw();
  }

  drawRunStatus(computer);

  drawSpeedBar(_runSpeed);

  buttons.setFrame((int)activeActionButtonA());
  gb.display.drawImage(73, 33, buttons);
}

