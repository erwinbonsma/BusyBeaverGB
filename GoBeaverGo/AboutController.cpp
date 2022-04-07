/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "AboutController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Challenges.h"
#include "Drawing.h"
#include "Store.h"

void AboutController::update() {
  if (gb.buttons.released(BUTTON_A)) {
    setController(&mainMenuController);
  }
}

char const* const labels[6] = {
  "Tutorials",
  "Challenges",
  "Longest run",
  "Lowest output",
  "Highest output",
  "Longest sequence"
};

void AboutController::draw() {
  gb.display.drawImage(8, 0, goBeaverGoImage);

  gb.display.setColor(DARKBLUE);
  gb.display.setCursor(22, 6);
  gb.display.print("by Eriban");

  gb.display.setColor(DARKBLUE);
  gb.display.fillRect(0, 30, 80, 34);
  gb.display.setColor(LIGHTBLUE);
  gb.display.drawRect(0, 30, 80, 34);
  gb.display.setCursor(18, 32);
  gb.display.setColor(ORANGE);
  gb.display.println("EXPERIMENTS");

  gb.display.setColor(BLUE);
  gb.display.setCursorY(16);
  for (int i = 0; i < 6; i++) {
    if (i == 2) {
      gb.display.setCursorY(38);
    }
    gb.display.setCursorX(2);
    gb.display.println(labels[i]);
  }

  gb.display.setColor(LIGHTBLUE);
  gb.display.setCursor(59, 16);
  gb.display.printf("%2d/%2d\n", getNumCompletedChallenges(true), tutorialsSet.size());
  gb.display.setCursorX(59);
  gb.display.printf("%2d/%2d\n", getNumCompletedChallenges(false), challengesSet.size());

  gb.display.setCursor(47, 38);
  gb.display.printf("%8d\n", getLongestRun());
  gb.display.setCursorX(47);
  gb.display.printf("%8d\n", getLowestOutput());
  gb.display.setCursorX(47);
  gb.display.printf("%8d\n", getHighestOutput());
  gb.display.setCursorX(47);
  gb.display.printf("%8d\n", getLongestSequence());
}
