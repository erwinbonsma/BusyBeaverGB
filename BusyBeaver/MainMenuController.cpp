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

const uint8_t goBeaverGoData[] = {
  64, 5, 1, 0, 1, 0xFF, 1,
  0xdd, 0xd0, 0x00, 0xdd, 0xdd, 0xd0, 0x00, 0xdd, 0xd0, 0x00, 0xdd, 0xdd, 0xd0, 0xdd, 0xdd, 0xd0, 0xd0, 0x00, 0xd0, 0xdd, 0xdd, 0xd0, 0xdd, 0xd0, 0x00, 0x00, 0xdd, 0xd0, 0x00, 0xdd, 0xdd, 0xd0,
  0xd0, 0x00, 0x00, 0xd0, 0x00, 0xd0, 0x00, 0xd0, 0xd0, 0x00, 0xd0, 0x00, 0x00, 0xd0, 0x00, 0xd0, 0xd0, 0x00, 0xd0, 0xd0, 0x00, 0x00, 0xd0, 0xd0, 0x00, 0x00, 0xd0, 0x00, 0x00, 0xd0, 0x00, 0xd0,
  0xd0, 0xdd, 0xd0, 0xd0, 0x00, 0xd0, 0x60, 0xdd, 0xdd, 0xd0, 0xdd, 0xd0, 0x00, 0xdd, 0xdd, 0xd0, 0xd0, 0xdd, 0xd0, 0xdd, 0xd0, 0x00, 0xdd, 0xdd, 0xd0, 0x60, 0xd0, 0xdd, 0xd0, 0xd0, 0x00, 0xd0,
  0xd0, 0x00, 0xd0, 0xd0, 0x00, 0xd0, 0x00, 0xd0, 0x00, 0xd0, 0xd0, 0x00, 0x00, 0xd0, 0x00, 0xd0, 0xd0, 0xd0, 0x00, 0xd0, 0x00, 0x00, 0xd0, 0x00, 0xd0, 0x00, 0xd0, 0x00, 0xd0, 0xd0, 0x00, 0xd0,
  0xdd, 0xdd, 0xd0, 0xdd, 0xdd, 0xd0, 0x00, 0xdd, 0xdd, 0xd0, 0xdd, 0xdd, 0xd0, 0xd0, 0x00, 0xd0, 0xdd, 0xd0, 0x00, 0xdd, 0xdd, 0xd0, 0xd0, 0x00, 0xd0, 0x00, 0xdd, 0xdd, 0xd0, 0xdd, 0xdd, 0xd0
};
Image goBeaverGoImage = Image(goBeaverGoData);

const uint8_t iconData[] = {
  32, 32, 1, 0, 1, 0xFF, 1,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x4f, 0x44, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0xf4, 0x44, 0x4d, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0x00, 0x00,
  0x04, 0x44, 0x44, 0x41, 0x11, 0x1d, 0x11, 0x11, 0x11, 0x1d, 0x11, 0x11, 0x11, 0x1d, 0x10, 0x00,
  0x04, 0x44, 0x44, 0x40, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x44, 0x44, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x66, 0x66, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x44, 0x44, 0x00,
  0x06, 0x67, 0x66, 0x60, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x04, 0x4f, 0x44, 0x40,
  0x06, 0x76, 0x66, 0x6c, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xdd, 0xdd, 0xd4, 0xf4, 0x44, 0x40,
  0x06, 0x66, 0x66, 0x61, 0x11, 0x1d, 0x11, 0x11, 0x11, 0x1c, 0x11, 0x11, 0x14, 0x44, 0x44, 0x40,
  0x06, 0x66, 0x66, 0x60, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0c, 0x10, 0x00, 0x04, 0x44, 0x44, 0x40,
  0x00, 0x66, 0x66, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0c, 0x10, 0x00, 0x00, 0x44, 0x44, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x66, 0x66, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x06, 0x67, 0x66, 0x60, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xd6, 0x76, 0x66, 0x6c, 0xcc, 0xcc, 0xcc, 0xcc,
  0x00, 0x0c, 0x11, 0x11, 0x11, 0x1d, 0x11, 0x11, 0x16, 0x66, 0x66, 0x61, 0x11, 0x1d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x06, 0x66, 0x66, 0x60, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x66, 0x66, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x0d, 0x10, 0x00,
  0x00, 0x0c, 0x10, 0x00, 0x04, 0x4f, 0x44, 0x40, 0x04, 0x4f, 0x44, 0x40, 0x00, 0x0d, 0x10, 0x00,
  0xcc, 0xcc, 0xdd, 0xdd, 0xd4, 0xf4, 0x44, 0x4d, 0xd4, 0xf4, 0x44, 0x4d, 0xdd, 0xdd, 0x10, 0x00,
  0x00, 0x00, 0x11, 0x11, 0x14, 0x44, 0x44, 0x41, 0x14, 0x44, 0x44, 0x41, 0x11, 0x11, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x44, 0x44, 0x40, 0x04, 0x44, 0x44, 0x40, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
Image iconImage = Image(iconData);

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
  gb.display.drawImage(10, 0, goBeaverGoImage);

  for (int i = 0; i < numMainMenuEntries; i++) {
    gb.display.setColor(i == _selectedEntry ? LIGHTBLUE : BLUE);
    gb.display.setCursor(22, 12 + i * 8);
    gb.display.print(mainMenuEntries[i]);
  }

  gb.display.drawImage(42, 32, iconImage);
  gb.display.setColor(LIGHTBLUE);
  gb.display.drawLine(77, 51, 70, 51);
  gb.display.drawLine(0, 59, 42, 59);
}

