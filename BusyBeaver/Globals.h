#ifndef __GLOBALS_INCLUDED
#define __GLOBALS_INCLUDED

#include "EditController.h"
#include "RunController.h"
#include "MainMenuController.h"

class Computer;
class ChallengeSpec;

extern Computer computer;

extern RunController runController;
extern EditController editController;
extern MainMenuController mainMenuController;

extern Controller* activeController;

#endif

