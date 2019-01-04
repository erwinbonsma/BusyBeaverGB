#include "Computer.h"

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
 * Where N = maxProgramsToStore
 */
#define BLOCK_SAVE_FILE_VERSION    0
#define BLOCK_NUM_STORED_PROGRAMS  1
#define BLOCK_PROGRAM_AUTO_NUM     2
#define BLOCK_PROGRAM_NAMES        3
#define BLOCK_CHALLENGE_BITMASK    4

/* Note: The number of program blocks may vary. Therefore it is best to add other blocks before
 * them. By leaving a gap in the indexing we reserve some room for future usage.
 */
#define BLOCK_FIRST_PROGRAM  16

#define SAVE_FILE_VERSION  1

const int maxProgramsToStore = 8;
const int maxProgramNameLength = 20; // Includes terminating \0
const int storedProgramSize = 17;
const int programIndexSize = maxProgramsToStore * maxProgramNameLength;

const SaveDefault savefileDefaults[5] = {
  { BLOCK_SAVE_FILE_VERSION, SAVETYPE_INT, 0, 0},
  { BLOCK_NUM_STORED_PROGRAMS, SAVETYPE_INT, 0, 0},
  { BLOCK_PROGRAM_AUTO_NUM, SAVETYPE_INT, 1, 0},
  { BLOCK_PROGRAM_NAMES, SAVETYPE_BLOB, programIndexSize, 0 },
  { BLOCK_CHALLENGE_BITMASK, SAVETYPE_INT, 0, 0},

  // Note: The size of the program BLOBS is specified using SAVECONF_DEFAULT_BLOBSIZE
};

char programIndexBuffer[programIndexSize];
uint8_t programBuffer[storedProgramSize];

char programNameBuffer[maxProgramNameLength];
char autoNameBuffer[maxProgramNameLength];

const char* programNames[maxProgramsToStore + 1];

void initSaveFileDefaults() {
  gb.save.config(savefileDefaults);
  if (gb.save.get(BLOCK_SAVE_FILE_VERSION) == 0) {
    // Version not set. Set it to current version to enable detecting future incompatibilities
    gb.save.set(BLOCK_SAVE_FILE_VERSION, SAVE_FILE_VERSION);
  }
}

int selectProgramSlot(bool store) {
  int numPrograms = gb.save.get(BLOCK_NUM_STORED_PROGRAMS);
  if (numPrograms == 0) {
    // No programs stored yet. Use first block
    return 0;
  }

  // Fill menu entries
  gb.save.get(BLOCK_PROGRAM_NAMES, (void*)programIndexBuffer, programIndexSize);
  int numEntries = 0;

  for (int i = 0; i < numPrograms; i++) {
    programNames[i] = (const char*)&programIndexBuffer[i * maxProgramNameLength];
    numEntries++;
  }

  if (store && numPrograms < maxProgramsToStore) {
    programNames[numEntries++] = "<Empty>";
  }

  programNames[numEntries++] = "Cancel";

  const char* menuName = store ? "Select Store Slot" : "Select Load Slot";
  int slot = gb.gui.menu(menuName, programNames, numEntries);
  return slot == (numEntries - 1) ? -1 : slot;
}

char* enterName() {
  // Set default name
  int autoNum = gb.save.get(BLOCK_PROGRAM_AUTO_NUM);
  snprintf(autoNameBuffer, maxProgramNameLength, "Program %d", autoNum);
  strncpy(programNameBuffer, autoNameBuffer, maxProgramNameLength);

  gb.gui.keyboard("Enter name", programNameBuffer, maxProgramNameLength - 1);

  if (strncmp(programNameBuffer, autoNameBuffer, maxProgramNameLength) == 0) {
    // Auto name was used so bump index to make next name unique
    gb.save.set(BLOCK_PROGRAM_AUTO_NUM, autoNum + 1);
  }

  return programNameBuffer;
}

void updateIndex(int slot, char* name) {
  // Update number of programs, if needed
  if ((slot + 1) > gb.save.get(BLOCK_NUM_STORED_PROGRAMS)) {
    gb.save.set(BLOCK_NUM_STORED_PROGRAMS, slot + 1);
  }

  // Copy name (assuming that programIndexBuffer is already filled by selectSlot)
  char* dst = &programIndexBuffer[slot * maxProgramNameLength];
  char* src = name;
  do {
    *(dst++) = *(src);
  } while (*(src++));

  gb.save.set(BLOCK_PROGRAM_NAMES, (void*)programIndexBuffer, programIndexSize);
}

bool storeProgram(int slot, char* name, Computer& computer) {
  updateIndex(slot, name);

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

bool storeProgram(Computer& computer) {
  int slot = selectProgramSlot(true);

  if (slot < 0) {
    // Aborted
    return false;
  }

  char* name = enterName();

  if (!storeProgram(slot, name, computer)) {
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

