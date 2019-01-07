/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

class Computer;

void initSaveFileDefaults();

int getMaxCompletedChallenge();
void setMaxCompletedChallenge(int index);

bool saveProgram(Computer& computer);
bool loadProgram(Computer& computer);

