/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __CONTROLLER_INCLUDED
#define __CONTROLLER_INCLUDED

class Controller {

public:
  virtual void activate() {};
  virtual void update() = 0;
  virtual void draw() = 0;
};

void setController(Controller* controller);

#endif
