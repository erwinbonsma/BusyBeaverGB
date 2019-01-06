#ifndef __MAIN_MENU_CONTROLLER_INCLUDED
#define __MAIN_MENU_CONTROLLER_INCLUDED

#include "Controller.h"

class MainMenuController : public Controller {
  int _selectedEntry = 0;

public:
  void update();
  void draw();
};

#endif
