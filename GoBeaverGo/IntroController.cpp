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

  if (gb.buttons.pressed(BUTTON_A)) {
    setController(&editController);
    editController.reset();
  }
}

void IntroController::draw() {
  const Challenge& challenge = challenges[activeChallenge];

  gb.display.setColor(DARKBLUE);
  gb.display.fillRect(0, 0, 80, 22);
  gb.display.setColor(BLUE);
  gb.display.drawRect(0, 0, 80, 22);

  gb.display.setColor(LIGHTBLUE);
  gb.display.setCursor(16, 4);
  gb.display.printf("Challenge %02d", (activeChallenge + 1));

  gb.display.setColor(WHITE);
  printCenter(challenge.name(), 12);

  gb.display.setColor(BLUE);
  printCenter("Goal:", 30);
  printCenter(challenge.typeDescription(), 38);

  if (_updateCount > 50) {
    gb.display.setColor(DARKBLUE);
    gb.display.fillRoundRect(22, 52, 36, 9, 2);
    gb.display.setColor(LIGHTBLUE);
    printCenter("Press A", 54);
  }
}

