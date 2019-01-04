/*
 * Busy Beaver, a 2L-based programming language and puzzle game
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Computer.h"
#include "Drawing.h"
#include "Store.h"
#include "EditController.h"
#include "RunController.h"

Computer computer(9);

RunController runController;
EditController editController;
Controller *activeController;

void setup() {
  gb.begin();

  initSaveFileDefaults();

  activeController = &editController;
}

void loop() {
  while(!gb.update());
  gb.display.clear();

  activeController->update();
  activeController->draw();
}

