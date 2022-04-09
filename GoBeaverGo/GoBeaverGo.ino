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

  if ( initSaveFileDefaults()) {
    gb.gui.popup("Save file patched", 40);
  }

  setController(&mainMenuController);
}

void loop() {
  while(!gb.update());
  gb.display.clear();

  activeController->update();
  activeController->draw();
}
