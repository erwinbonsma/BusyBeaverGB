/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __HELP_CONTROLLER_INCLUDED
#define __HELP_CONTROLLER_INCLUDED

#include "Controller.h"

class HelpController : public Controller {
  int _page;

public:
  void activate();
  void update();
  void draw();
};

#endif
