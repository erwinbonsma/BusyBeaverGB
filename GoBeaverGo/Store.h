/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __STORE_INCLUDED
#define __STORE_INCLUDED

#include <Gamebuino-Meta.h>

class Computer;

constexpr int programStorageSize = 64;
constexpr int maxProgramNameLength = 20; // Includes terminating \0

// 5 instructions per byte (as 5 * 3 = 243 < 255)
// 17 * 5 = 85 > 9 * 9
constexpr int storedProgramSize = 17;
constexpr int programIndexSize = programStorageSize * maxProgramNameLength;

struct StorageManagement {
  // Stores the names of all saved programs
  char programIndexBuffer[programIndexSize];

  // Used to read/write a program
  uint8_t programBuffer[storedProgramSize];

  // Used to edit a program's name
  char programNameBuffer[maxProgramNameLength];

  // Used for generating a default program name
  char autoNameBuffer[maxProgramNameLength];

  // Used for generating the program selection menu
  const char* programNames[programStorageSize + 1];
};

bool initSaveFileDefaults();

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

#endif
