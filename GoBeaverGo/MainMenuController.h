/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __MAIN_MENU_CONTROLLER_INCLUDED
#define __MAIN_MENU_CONTROLLER_INCLUDED

#include <Gamebuino-Meta.h>

#include "Controller.h"

constexpr int maxChallengeMenuEntries = 32;

struct MainMenuMemory {
  const char* challengeMenuEntries[maxChallengeMenuEntries];
  char challengesMenuTitleBuf[20];
};

class MainMenuController : public Controller {
  int _selectedEntry = 0;
  bool _ignoreButtonRelease;

public:
  void activate();
  void update();
  void draw();
};

extern Image goBeaverGoImage;

#endif
