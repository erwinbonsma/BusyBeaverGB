/*
 * Busy Beaver, a 2L-based programming language and puzzle game
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Program.h"
#include "Drawing.h"

Program program(9);

const int unitRunSpeed = 8;

int runSpeed = 0;
int stepPeriod = 0;
int stepsPerTick = 0;
int ticksSinceLastStep = 0;

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

void changeRunSpeed(int delta) {
  runSpeed = max(0, runSpeed + delta);

  if (runSpeed == 0) {
    return;
  }

  if (runSpeed < unitRunSpeed) {
    stepPeriod = 1 << (unitRunSpeed - runSpeed);
    stepsPerTick = 1;
  } else {
    stepPeriod = 1;
    stepsPerTick = 1 << (runSpeed - unitRunSpeed);
  }
}

void setup() {
  gb.begin();

  program.setProgram(busyBeaver);
  //program.setProgram(buggy);
}

void loop() {
  while(!gb.update());
  gb.display.clear();

  if (gb.buttons.pressed(BUTTON_UP)) {
    changeRunSpeed(+1);
  }
  else if (gb.buttons.pressed(BUTTON_DOWN)) {
    changeRunSpeed(-1);
  }
  else if (gb.buttons.pressed(BUTTON_A)) {
    program.step();
  }
  else if (gb.buttons.pressed(BUTTON_B)) {
    program.reset();
  }

  if (runSpeed > 0) {
    if (++ticksSinceLastStep >= stepPeriod) {
      for (int i = 0; i < stepsPerTick; i++) {
        program.step();
        ticksSinceLastStep = 0;
      }
    }
  }

  drawProgram(program);
}

