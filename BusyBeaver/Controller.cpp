#include "Controller.h"

#include "Globals.h"

void setController(Controller* controller) {
  activeController  = controller;
  controller->activate();
}

