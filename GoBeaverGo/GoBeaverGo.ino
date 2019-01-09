/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Store.h"
#include "Globals.h"

Computer computer;

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

