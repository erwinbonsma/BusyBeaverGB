/*
 * Busy Beaver, a 2L-based programming language and puzzle game
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Store.h"
#include "Globals.h"

Computer computer(9);

void setup() {
  gb.begin();

  initSaveFileDefaults();

  setController(&mainMenuController);
}

void loop() {
  while(!gb.update());
  gb.display.clear();

  activeController->update();
  activeController->draw();
}

