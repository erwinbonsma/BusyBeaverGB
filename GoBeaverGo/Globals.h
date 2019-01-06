#ifndef __GLOBALS_INCLUDED
#define __GLOBALS_INCLUDED

#include "EditController.h"
#include "RunController.h"
#include "MainMenuController.h"
#include "IntroController.h"
#include "HelpController.h"

class Computer;
extern Computer computer;

extern RunController runController;
extern EditController editController;
extern MainMenuController mainMenuController;
extern IntroController introController;
extern HelpController helpController;

extern Controller* activeController;

#endif

