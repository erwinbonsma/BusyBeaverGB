/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include "Store.h"

#include "Computer.h"
#include "SharedMemory.h"

/* Block layout
 *
 * 0: INT, Save file format version
 * 1: INT, Number of stored programs
 * 2: INT, Next program ID
 * 3: Program names index
 * 4: INT, Challenge bitmask
 *
 * 16 .. 16+N-1: Programs
 *
 * Where N = programStorageSize
 */
#define BLOCK_SAVE_FILE_VERSION    0
#define BLOCK_NUM_STORED_PROGRAMS  1
#define BLOCK_PROGRAM_AUTO_NUM     2
#define BLOCK_PROGRAM_NAMES        3
#define BLOCK_CHALLENGE_TRACKER    4
#define BLOCK_TUTORIAL_TRACKER     5
#define BLOCK_LONGEST_RUN          6
#define BLOCK_LOWEST_OUTPUT        7
#define BLOCK_HIGHEST_OUTPUT       8
#define BLOCK_LONGEST_SEQUENCE     9

/* Note: The number of program blocks may vary. Therefore it is best to add other blocks before
 * them. By leaving a gap in the indexing we reserve some room for future usage.
 */
#define BLOCK_FIRST_PROGRAM  16

#define SAVE_FILE_VERSION  4

const SaveDefault savefileDefaults[10] = {
  { BLOCK_SAVE_FILE_VERSION, SAVETYPE_INT, SAVE_FILE_VERSION, 0},
  { BLOCK_NUM_STORED_PROGRAMS, SAVETYPE_INT, 0, 0},
  { BLOCK_PROGRAM_AUTO_NUM, SAVETYPE_INT, 1, 0},
  { BLOCK_PROGRAM_NAMES, SAVETYPE_BLOB, programIndexSize, 0 },
  { BLOCK_CHALLENGE_TRACKER, SAVETYPE_INT, 0, 0},
  { BLOCK_TUTORIAL_TRACKER, SAVETYPE_INT, 0, 0},
  { BLOCK_LONGEST_RUN, SAVETYPE_INT, 0, 0},
  { BLOCK_LOWEST_OUTPUT, SAVETYPE_INT, 0, 0},
  { BLOCK_HIGHEST_OUTPUT, SAVETYPE_INT, 0, 0},
  { BLOCK_LONGEST_SEQUENCE, SAVETYPE_INT, 0, 0}

  // Note: The size of the program BLOBS is specified using SAVECONF_DEFAULT_BLOBSIZE
};

bool initSaveFileDefaults() {
  gb.save.config(16 + programStorageSize, savefileDefaults);

  if (gb.save.get(BLOCK_SAVE_FILE_VERSION) == SAVE_FILE_VERSION) {
    return false;
  }

  // Wipe all challenge progress data as in Version 4 big changes were made to
  // challenges (new ones added, old ones updated, order changed, etc).
  gb.save.set(BLOCK_CHALLENGE_TRACKER, (int)0);

  gb.save.set(BLOCK_SAVE_FILE_VERSION, SAVE_FILE_VERSION);

  return true;
}

int getNumCompletedChallenges(bool isTutorial) {
  int blockIndex = isTutorial ? BLOCK_TUTORIAL_TRACKER : BLOCK_CHALLENGE_TRACKER;
  int bitMask = gb.save.get(blockIndex);

  int num = 0;
  while (bitMask != 0) {
    if ((bitMask & 1) != 0) {
      num++;
    }
    bitMask >>= 1;
  }

  return num;
}

void recordChallengeCompleted(int index, bool isTutorial) {
  int blockIndex = isTutorial ? BLOCK_TUTORIAL_TRACKER : BLOCK_CHALLENGE_TRACKER;
  int bitMask = gb.save.get(blockIndex);

  bitMask |= 1 << index;

  gb.save.set(blockIndex, bitMask);
}

void recordExperimentDone(Computer& computer) {
  if (computer.getNumSteps() > getLongestRun()) {
    gb.save.set(BLOCK_LONGEST_RUN, computer.getNumSteps());
  }

  int output = computer.getOutput();
  if (output < getLowestOutput()) {
    gb.save.set(BLOCK_LOWEST_OUTPUT, output);
  }
  else if (output > getHighestOutput()) {
    gb.save.set(BLOCK_HIGHEST_OUTPUT, output);
  }

  // Find first non-zero value
  int low = computer.getMinDataAddress();
  while (low <= computer.getMaxDataAddress() && computer.getData(low) == 0) {
    low++;
  }

  // Find last non-zero value
  int hi = computer.getMaxDataAddress();
  while (hi >= computer.getMinDataAddress() && computer.getData(hi) == 0) {
    hi--;
  }

  int len = hi - low + 1;
  if (len > getLongestSequence()) {
    gb.save.set(BLOCK_LONGEST_SEQUENCE, len);
  }
}

int getLongestRun() {
  return gb.save.get(BLOCK_LONGEST_RUN);
}

int getLowestOutput() {
  return gb.save.get(BLOCK_LOWEST_OUTPUT);
}

int getHighestOutput() {
  return gb.save.get(BLOCK_HIGHEST_OUTPUT);
}

int getLongestSequence() {
  return gb.save.get(BLOCK_LONGEST_SEQUENCE);
}

int selectProgramSlot(bool store) {
  int numPrograms = gb.save.get(BLOCK_NUM_STORED_PROGRAMS);
  if (numPrograms == 0) {
    // No programs stored yet. Use first block
    return 0;
  }

  auto &programIndexBuffer = shared_mem.storage.programIndexBuffer;
  auto &programNames = shared_mem.storage.programNames;

  // Fill menu entries
  gb.save.get(BLOCK_PROGRAM_NAMES, (void*)programIndexBuffer, programIndexSize);
  int numEntries = 0;

  for (int i = 0; i < numPrograms; i++) {
    programNames[i] = (const char*)&programIndexBuffer[i * maxProgramNameLength];
    numEntries++;
  }

  if (store && numPrograms < programStorageSize) {
    programNames[numEntries++] = "<Empty>";
  }

  programNames[numEntries++] = "Cancel";

  const char* menuName = store ? "Select Store Slot" : "Select Load Slot";
  int slot = gb.gui.menu(menuName, programNames, numEntries);
  return slot == (numEntries - 1) ? -1 : slot;
}

char* enterName(int slot) {
  auto &programNames = shared_mem.storage.programNames;
  auto &programNameBuffer = shared_mem.storage.programNameBuffer;
  auto &autoNameBuffer = shared_mem.storage.autoNameBuffer;

  int numPrograms = gb.save.get(BLOCK_NUM_STORED_PROGRAMS);
  bool overwrite = slot < numPrograms;
  if (overwrite) {
    // Select name of program to be overwritten (to facilitate repeated saving of a program)
    strncpy(programNameBuffer, programNames[slot], maxProgramNameLength);
  } else {
    // Set default name
    int autoNum = gb.save.get(BLOCK_PROGRAM_AUTO_NUM);
    snprintf(autoNameBuffer, maxProgramNameLength, "Program %d", autoNum);
    strncpy(programNameBuffer, autoNameBuffer, maxProgramNameLength);
  }

  gb.gui.keyboard("Enter name", programNameBuffer, maxProgramNameLength - 1);

  if (!overwrite && strncmp(programNameBuffer, autoNameBuffer, maxProgramNameLength) == 0) {
    // Auto name was used so bump index to make next name unique
    gb.save.set(BLOCK_PROGRAM_AUTO_NUM, gb.save.get(BLOCK_PROGRAM_AUTO_NUM) + 1);
  }

  return programNameBuffer;
}

void updateIndex(int slot, char* name) {
  // Update number of programs, if needed
  if ((slot + 1) > gb.save.get(BLOCK_NUM_STORED_PROGRAMS)) {
    gb.save.set(BLOCK_NUM_STORED_PROGRAMS, slot + 1);
  }

  auto &programIndexBuffer = shared_mem.storage.programIndexBuffer;

  // Copy name (assuming that programIndexBuffer is already filled by selectSlot)
  char* dst = &programIndexBuffer[slot * maxProgramNameLength];
  char* src = name;
  do {
    *dst++ = *src;
  } while (*src++);

  gb.save.set(BLOCK_PROGRAM_NAMES, (void*)programIndexBuffer, programIndexSize);
}

bool saveProgram(int slot, char* name, Computer& computer) {
  updateIndex(slot, name);

  auto &programBuffer = shared_mem.storage.programBuffer;

  int p_out = 0;
  int shifts = 0;
  int p_in = 0;
  int outval = 0;
  int numCells = maxProgramSize * maxProgramSize;

  do {
    int x = p_in % 9;
    int y = (p_in - x) / 9;
    int instruction = (int)computer.getInstruction(x, y);

    outval += instruction;
    if (shifts == 4) {
      programBuffer[p_out++] = outval;
      shifts = 0;
      outval = 0;
    } else {
      outval *= 3;
      shifts++;
    }
  } while (++p_in < numCells);

  while (shifts++ < 4) {
    outval *= 3;
  }

  gb.save.set(BLOCK_FIRST_PROGRAM + slot, (void*)programBuffer, storedProgramSize);

  return true;
}

bool loadProgram(int slot, Computer& computer) {
  auto &programBuffer = shared_mem.storage.programBuffer;

  gb.save.get(BLOCK_FIRST_PROGRAM + slot, (void*)programBuffer, storedProgramSize);

  int p_out = 0;
  int p_in = 0;

  do {
    int inval = programBuffer[p_in++];

    int p_dst = p_out + 4;
    do {
      int x = p_dst % 9;
      int y = (p_dst - x) / 9;
      int int_ins = inval % 3;
      inval = (inval - int_ins) / 3;

      if (y < maxProgramSize) {
        computer.setInstruction(x, y, (Instruction)int_ins);
      }
    } while (--p_dst >= p_out);

    p_out += 5;
  } while (p_in < storedProgramSize);

  return true;
}

bool saveProgram(Computer& computer) {
  int slot = selectProgramSlot(true);

  if (slot < 0) {
    // Aborted
    return false;
  }

  char* name = enterName(slot);

  if (!saveProgram(slot, name, computer)) {
    gb.gui.popup("Save failed!", 40);
    return false;
  }

  gb.gui.popup("Program saved", 40);
  return true;
}

bool loadProgram(Computer& computer) {
  int slot = selectProgramSlot(false);

  if (slot < 0) {
    // Aborted
    return false;
  }

  if (!loadProgram(slot, computer)) {
    gb.gui.popup("Load failed", 40);
  }

  return true;
}
