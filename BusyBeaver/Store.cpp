#include "Computer.h"

const int indexBlock = 1;

const int maxProgramNameLength = 20; // Includes terminating \0
const int storedProgramSize = 17;
const int maxProgramsToStore = 8;
const int programIndexSize = (
  // Number of programs stored
  1 +
  // Next Auto-number index
  1 +
  // Names of each program
  maxProgramsToStore * maxProgramNameLength
);

char programIndexBuffer[programIndexSize];
uint8_t programBuffer[storedProgramSize];

char programNameBuffer[maxProgramNameLength];
char autoNameBuffer[maxProgramNameLength];

const char* programNames[maxProgramsToStore + 1];

int selectProgramSlot(bool store) {
  if ( !gb.save.get(indexBlock, (void*)programIndexBuffer, programIndexSize)) {
    // Could not read index

    // Set some defaults
    programIndexBuffer[0] = 0; // No programs stored yet
    programIndexBuffer[1] = 0; // Reset auto-number count

    // Select first slot
    return 0;
  }

  // Fill menu entries
  int numNames = programIndexBuffer[0];
  int numEntries = 0;

  for (int i = 0; i < numNames; i++) {
    programNames[i] = (const char*)&programIndexBuffer[2 + i * maxProgramNameLength];
    numEntries++;
  }

  if (store && numEntries < maxProgramsToStore) {
    programNames[numEntries++] = "<Empty>";
  }

  programNames[numEntries++] = "Cancel";

  const char* menuName = store ? "Select Store Slot" : "Select Load Slot";
  int slot = gb.gui.menu(menuName, programNames, numEntries);
  return slot == (numEntries - 1) ? -1 : slot;
}

char* enterName() {
  // Set default name
  int autoNum = programIndexBuffer[1] + 1;
  snprintf(autoNameBuffer, maxProgramNameLength, "Program %d", autoNum);
  strncpy(programNameBuffer, autoNameBuffer, maxProgramNameLength);

  gb.gui.keyboard("Enter name", programNameBuffer, maxProgramNameLength - 1);

  if (strncmp(programNameBuffer, autoNameBuffer, maxProgramNameLength) == 0) {
    // Auto name was used so bump index to make next name unique
    programIndexBuffer[1] += 1;
  }

  return programNameBuffer;
}

void updateIndex(int slot, char* name) {
  // Assumes programIndexBuffer already filled by selectSlot

  // Update number of programs, if needed
  programIndexBuffer[0] = max(programIndexBuffer[0], slot + 1);

  // Copy name
  char* dst = &programIndexBuffer[2 + slot * maxProgramNameLength];
  char* src = name;
  do {
    *(dst++) = *(src);
  } while (*(src++));

  gb.save.set(indexBlock, (void*)programIndexBuffer, programIndexSize);
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

  gb.save.set(slot + 2, (void*)programBuffer, storedProgramSize);

  return true;
}

bool loadProgram(int slot, Computer& computer) {
  gb.save.get(slot + 2, (void*)programBuffer, storedProgramSize);

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

