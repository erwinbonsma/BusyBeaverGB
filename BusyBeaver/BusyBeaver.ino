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

int busyBeaver[81] = {
  0,1,1,0,0,0,0,0,0,
  0,2,2,0,0,0,1,1,0,
  0,2,2,1,2,2,2,0,1,
  0,2,2,0,0,0,2,0,0,
  0,2,2,0,0,0,2,0,1,
  1,0,2,0,0,0,2,0,1,
  1,2,2,0,1,1,2,0,0,
  1,1,0,0,0,0,0,0,0,
  2,2,2,2,2,2,2,1,0
};

// For testing purposes
int buggy[81] = {
  0,1,1,0,0,0,0,0,0,
  0,2,2,0,0,0,1,1,0,
  0,2,2,1,2,2,2,0,1,
  0,2,2,0,0,0,2,0,0,
  0,2,2,0,0,0,1,0,1,
  1,2,2,2,2,2,0,0,1,
  1,2,2,0,1,1,0,0,0,
  1,1,0,0,0,0,0,0,0,
  2,0,0,0,0,0,2,1,0
};

void setup() {
  gb.begin();

  initSaveFileDefaults();

  computer.setProgram(busyBeaver);
  //computer.setProgram(buggy);

  //activeController = &runController;
  activeController = &editController;
}

void loop() {
  while(!gb.update());
  gb.display.clear();

  activeController->update();
  activeController->draw();
}

