#ifndef __GLOBALS_INCLUDED
#define __GLOBALS_INCLUDED

#include "EditController.h"
#include "RunController.h"

class Computer;

extern Computer computer;

extern RunController runController;
extern EditController editController;
extern Controller *activeController;

#endif

