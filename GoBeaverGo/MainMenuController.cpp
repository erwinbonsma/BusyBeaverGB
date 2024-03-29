/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "MainMenuController.h"

#include "Globals.h"
#include "Challenges.h"
#include "Drawing.h"
#include "SharedMemory.h"
#include "Store.h"

constexpr int numMainMenuEntries = 5;
char const* const mainMenuEntries[numMainMenuEntries] = {
  "Tutorial",
  "Challenges",
  "Experiment",
  "Stats",
  "Help"
};

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

bool selectChallenge() {
  auto& challengeMenuEntries = shared_mem.main_menu.challengeMenuEntries;
  auto& challengesMenuTitleBuf = shared_mem.main_menu.challengesMenuTitleBuf;

  int numOptions;

  if (activeChallengeSet == &challengesSet) {
    numOptions = min(
      getNumCompletedChallenges(false) + 1,
      activeChallengeSet->size()
    );
  } else {
    numOptions = activeChallengeSet->size();
  }

  for (int i = 0; i < numOptions; i++) {
    challengeMenuEntries[i] = activeChallengeSet->challengeAt(i)->name();
  }
  challengeMenuEntries[numOptions] = "Cancel";

  snprintf(
    challengesMenuTitleBuf, sizeof(challengesMenuTitleBuf),
    "Select %s", activeChallengeSet->challengeType()
  );
  int selected = gb.gui.menu(challengesMenuTitleBuf, challengeMenuEntries, numOptions + 1);

  if (selected == numOptions) {
    // Cancel
    return false;
  } else {
    activeChallenge = activeChallengeSet->challengeAt(selected);
    return true;
  }
}

void MainMenuController::activate() {
  _ignoreButtonRelease = gb.buttons.pressed(BUTTON_A);
}

void MainMenuController::update() {
  if (gb.buttons.pressed(BUTTON_DOWN)) {
    _selectedEntry = (_selectedEntry + 1) % numMainMenuEntries;
  }
  else if (gb.buttons.pressed(BUTTON_UP)) {
    _selectedEntry = (_selectedEntry + numMainMenuEntries - 1) % numMainMenuEntries;
  }
  // Need to check for release as that is also the trigger inside gb.gui.menu.
  // This avoids that the same button press also triggers menu selection there.
  else if (gb.buttons.released(BUTTON_A)) {
    if (_ignoreButtonRelease) {
      _ignoreButtonRelease = false;
    } else switch (_selectedEntry) {
      case 0:
        activeChallengeSet = &tutorialsSet;
        runController.setRunSpeed(2);
        if (selectChallenge()) {
          setController(&introController);
        }
        break;
      case 1:
        activeChallengeSet = &challengesSet;
        runController.setRunSpeed(4);
        if (selectChallenge()) {
          setController(&introController);
        }
        break;
      case 2:
        activeChallengeSet = nullptr;
        activeChallenge = nullptr;
        setController(&editController);
        editController.reset();
        break;
      case 3:
        setController(&statsController);
        break;
      case 4:
        setController(&helpController);
        break;
      default:
        break;
    }
  }
}

void MainMenuController::draw() {
  gb.display.drawImage(8, 0, goBeaverGoImage);

  for (int i = 0; i < numMainMenuEntries; i++) {
    gb.display.setColor(i == _selectedEntry ? LIGHTBLUE : BLUE);
    gb.display.setCursor(20, 9 + i * 8);
    gb.display.print(mainMenuEntries[i]);
  }

  gb.display.drawImage(42, 32, iconImage);
  gb.display.setColor(LIGHTBLUE);
  gb.display.drawLine(77, 51, 70, 51);
  gb.display.drawLine(0, 59, 42, 59);
}
