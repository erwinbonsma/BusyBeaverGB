/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "IntroController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Challenges.h"

void printCenter(const char* text, int y) {
  int len = strlen(text);
  gb.display.setCursor(40 - 2 * len, y);
  gb.display.print(text);
}

void IntroController::activate() {
  _updateCount = 0;
}

void IntroController::update() {
  _updateCount++;

  if (gb.buttons.released(BUTTON_A)) {
    setController(&editController);
    editController.reset();
  }
}

char challengeTitleBuf[16];
void IntroController::draw() {
  gb.display.setColor(DARKBLUE);
  gb.display.fillRect(0, 0, 80, 22);
  gb.display.setColor(BLUE);
  gb.display.drawRect(0, 0, 80, 22);

  gb.display.setColor(LIGHTBLUE);
  snprintf(
    challengeTitleBuf, sizeof(challengeTitleBuf),
    "%s %d",
    activeChallengeSet->challengeType(),
    activeChallengeSet->indexOfChallenge(activeChallenge) + 1
  );
  printCenter(challengeTitleBuf, 4);

  gb.display.setColor(WHITE);
  printCenter(activeChallenge->name(), 12);

  gb.display.setColor(BLUE);
  printCenter("Goal:", 30);
  printCenter(activeChallenge->typeDescription(), 38);

  if (_updateCount > 50) {
    gb.display.setColor(DARKBLUE);
    gb.display.fillRoundRect(22, 52, 36, 9, 2);
    gb.display.setColor(LIGHTBLUE);
    printCenter("Press A", 54);
  }
}
