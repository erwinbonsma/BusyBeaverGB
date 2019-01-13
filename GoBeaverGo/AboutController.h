/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __ABOUT_CONTROLLER_INCLUDED
#define __ABOUT_CONTROLLER_INCLUDED

#include "Controller.h"

class AboutController : public Controller {

public:
  void update();
  void draw();
};

#endif
