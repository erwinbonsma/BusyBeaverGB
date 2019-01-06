/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "Controller.h"

#include "Globals.h"

void setController(Controller* controller) {
  activeController  = controller;
  controller->activate();
}

