#include "MainMenuController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Challenges.h"
#include "Store.h"

const int numMainMenuEntries = 3;
const char* mainMenuEntries[numMainMenuEntries] = {
  "Challenges",
  "Experiment",
  "Help"
};

const char* challengeEntries[numChallenges];

void challengesMenu() {
  int numOptions = min(getMaxCompletedChallenge() + 1, numChallenges);
  for (int i = 0; i < numOptions; i++) {
    challengeEntries[i] = challenges[i].name();
  }

  int selected = gb.gui.menu("Select challenge", challengeEntries, numOptions);

  activeChallenge = selected;
  setController(&editController);
  editController.reset();
}

void MainMenuController::update() {
  if (gb.buttons.pressed(BUTTON_DOWN)) {
    _selectedEntry = (_selectedEntry + 1) % numMainMenuEntries;
  }
  else if (gb.buttons.pressed(BUTTON_UP)) {
    _selectedEntry = (_selectedEntry + numMainMenuEntries - 1) % numMainMenuEntries;
  }
  else if (gb.buttons.pressed(BUTTON_A)) {
    switch (_selectedEntry) {
      case 0:
        challengesMenu();
        break;
      case 1:
        activeChallenge = NO_CHALLENGE;
        setController(&editController);
        editController.reset();
        break;
      case 2:
        // TO DO
        gb.gui.popup("Help!", 40);
        break;
      default:
        break;
    }
  }
}

void MainMenuController::draw() {
  for (int i = 0; i < numMainMenuEntries; i++) {
    gb.display.setColor(i == _selectedEntry ? LIGHTBLUE : BLUE);
    gb.display.setCursor(10, i * 8);
    gb.display.print(mainMenuEntries[i]);
  }
}

