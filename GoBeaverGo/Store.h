/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

class Computer;

void initSaveFileDefaults();

// Signals that the given challenge has been completed.
void recordChallengeCompleted(int index, bool isTutorial);

// Returns the number of completed challenges.
int getNumCompletedChallenges(bool isTutorial);

/* Signals that the current experimental program has completed. It will check if this program has
 * broken any records, and if so, record these.
 */
void recordExperimentDone(Computer& computer);

int getLongestRun();
int getLowestOutput();
int getHighestOutput();
int getLongestSequence();

bool saveProgram(Computer& computer);
bool loadProgram(Computer& computer);

