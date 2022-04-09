/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "Globals.h"

RunController runController;
EditController editController;
MainMenuController mainMenuController;
IntroController introController;
StatsController statsController;
HelpController helpController;

Controller *activeController;
