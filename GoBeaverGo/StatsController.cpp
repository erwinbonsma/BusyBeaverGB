/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "StatsController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Challenges.h"
#include "Drawing.h"
#include "SharedMemory.h"
#include "Store.h"

void StatsController::update() {
  if (gb.buttons.released(BUTTON_A)) {
    setController(&mainMenuController);
  }
}

char const* const labels[6] = {
  "Tutorials",
  "Challenges",
  "Min output",
  "Max output",
  "Max runlen",
  "Max seqlen"
};

void StatsController::activate() {
  shared_mem.stats.min_value = getLowestOutput();
  shared_mem.stats.max_value = getHighestOutput();
  shared_mem.stats.max_run_len = getLongestRun();
  shared_mem.stats.max_seq_len = getLongestSequence();
  shared_mem.stats.num_completed_tutorials = getNumCompletedChallenges(true);
  shared_mem.stats.num_completed_challenges = getNumCompletedChallenges(false);
}

void StatsController::draw() {
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
  gb.display.printf("%2d/%2d\n", shared_mem.stats.num_completed_tutorials, tutorialsSet.size());
  gb.display.setCursorX(59);
  gb.display.printf("%2d/%2d\n", shared_mem.stats.num_completed_challenges, challengesSet.size());

  gb.display.setCursor(47, 38);
  gb.display.printf("%8d\n", shared_mem.stats.min_value);
  gb.display.setCursorX(47);
  gb.display.printf("%8d\n", shared_mem.stats.max_value);
  gb.display.setCursorX(47);
  gb.display.printf("%8d\n", shared_mem.stats.max_run_len);
  gb.display.setCursorX(47);
  gb.display.printf("%8d\n", shared_mem.stats.max_seq_len);
}
