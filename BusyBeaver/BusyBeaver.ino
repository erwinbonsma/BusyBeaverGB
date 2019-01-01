/*
 * Busy Beaver, a 2L-based programming language and puzzle game
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Program.h"
#include "Drawing.h"

Program program(9);

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

void setup() {
  gb.begin();

  program.setProgram(busyBeaver);
}

void loop() {
  while(!gb.update());
  gb.display.clear();

  drawProgram(program);
}

