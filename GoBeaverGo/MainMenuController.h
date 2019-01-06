/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __MAIN_MENU_CONTROLLER_INCLUDED
#define __MAIN_MENU_CONTROLLER_INCLUDED

#include <Gamebuino-Meta.h>

#include "Controller.h"

class MainMenuController : public Controller {
  int _selectedEntry = 0;

public:
  void update();
  void draw();
};

extern Image goBeaverGoImage;

#endif
