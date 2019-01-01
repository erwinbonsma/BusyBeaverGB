#ifndef __GLOBALS_INCLUDED
#define __GLOBALS_INCLUDED

#include "EditController.h"
#include "RunController.h"

class Program;

extern Program program;

extern RunController runController;
extern EditController editController;
extern Controller *activeController;

#endif

