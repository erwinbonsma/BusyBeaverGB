/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Drawing.h"

#include "Globals.h"
#include "Computer.h"
#include "RunController.h"

int board_x0;
int board_y0;

// Used by drawData to move data tape with a certain inertia
float lastCenterAddress = 0;
float dataTapeInertia = 0.8f;

void resetDrawing() {
  int pixelSize = computer.getSize() * 5;
  board_x0 = (80 - pixelSize) / 2;
  board_y0 = (64 - pixelSize) / 2 - 2;

  lastCenterAddress = 0;
}

int getDisplayX(int addressX) {
  return board_x0 + 5 * addressX;
}

int getDisplayY(int addressY) {
  return board_y0 + 5 * (computer.getSize() - addressY - 1);
}

void drawSpeedBar(int speed) {
  int emptyLen = (maxRunSpeed - speed) * 2;
  if (emptyLen > 0) {
    gb.display.setColor(DARKBLUE);
    gb.display.fillRect(1, 9, 3, emptyLen);
  }

  gb.display.setColor(BLUE);
  gb.display.fillRect(1, 9 + emptyLen, 3, (speed + 1) * 2);
}

void drawCursor(int x, int y) {
  gb.display.setColor(BLUE);
  gb.display.drawRect(getDisplayX(x), getDisplayY(y), 5, 5);
}

void drawProgramPointer(Computer& computer) {
  ProgramPointer pp = computer.getProgramPointer();
  int x0 = getDisplayX(pp.x);
  int y0 = getDisplayY(pp.y);
  int w = 4;

  if (computer.getStatus() == Status::Done) {
    x0--;
    y0--;
    w += 2;
  }

  gb.display.setColor(ORANGE);
  gb.display.drawRect(x0, y0, w + 1, w + 1);

  gb.display.setColor(YELLOW);
  switch (pp.dir) {
    case Direction::Up:
      gb.display.drawLine(x0, y0, x0 + w, y0);
      break;
    case Direction::Down:
      gb.display.drawLine(x0, y0 + w, x0 + w, y0 + w);
      break;
    case Direction::Left:
      gb.display.drawLine(x0, y0, x0, y0 + w);
      break;
    case Direction::Right:
      gb.display.drawLine(x0 + w, y0, x0 + w, y0 + w);
      break;
  }
}

int getWidthOfValue(int val) {
  int numChars = 0;
  if (val < 0) {
    val = -val;
    numChars++; // Also count minus sign
  }

  do {
    val -= (val % 10);
    val /= 10;
    numChars++;
  } while (val != 0);

  return numChars * 4;
}

int getWidthOfAddress(int address) {
  if (
    address < computer.getMinDataAddress() ||
    address > computer.getMaxDataAddress()
  ) {
    return 4;
  } else {
    return getWidthOfValue(computer.getData(address));
  }
}

void drawData(Computer& computer, int desiredCenterAddress) {
  lastCenterAddress = (1 - dataTapeInertia) * desiredCenterAddress + dataTapeInertia * lastCenterAddress;
  int lowAddress = (int)floorf(lastCenterAddress);
  int highAddress = lowAddress + 1;

  int address = lowAddress;
  int x = 40 - getWidthOfAddress(lowAddress) / 2;
  x -= (int)(
    (lastCenterAddress - lowAddress) *
    (2 + (getWidthOfAddress(lowAddress) + getWidthOfAddress(highAddress)) / 2)
  );

  // Find left-most data cell that fits on the screen
  bool done = false;
  while (!done) {
    int delta = getWidthOfAddress(address) + 2;
    if (x - delta < 0) {
      done = true;
    } else {
      x -= delta;
      address--;
    }
  }

  gb.display.setCursorY(59);
  while (x < 80) {
    int w;
    gb.display.setCursorX(x);
    gb.display.setColor(address == computer.getDataPointer() ? LIGHTGREEN : GREEN);

    if (address < computer.getMinDataAddress()) {
      char ch = (computer.getMaxDataAddress() - address < dataSize) ? '0' : '*';
      w = 4;
      if (x + w < 80) {
        gb.display.print(ch);
      }
    }
    else if (address > computer.getMaxDataAddress()) {
      char ch = (address - computer.getMinDataAddress() < dataSize) ? '0' : '*';
      w = 4;
      if (x + w < 80) {
        gb.display.print(ch);
      }
    }
    else {
      int val = computer.getData(address);
      w = getWidthOfValue(val);
      if (x + w < 80) {
        gb.display.print(val, DEC);
      }
    }

    x += w + 2;
    address++;
  }
}

void drawProgramSpace() {
  // Grid lines
  gb.display.setColor(DARKBLUE);
  int lineLen = (computer.getSize() - 1) * 5;
  for (int i = 0; i < computer.getSize(); i++) {
    gb.display.drawLine(
      board_x0 + 2 + 5 * i, board_y0 + 2,
      board_x0 + 2 + 5 * i, board_y0 + 2 + lineLen
    );
    gb.display.drawLine(
      board_x0 + 2,           board_y0 + 2 + 5 * i,
      board_x0 + 2 + lineLen, board_y0 + 2 + 5 * i
    );
  }
}

void drawInstruction(int displayX, int displayY, Instruction instruction) {
  if (instruction != Instruction::Noop) {
    gb.display.setColor(instruction == Instruction::Turn ? BROWN: GRAY);
    gb.display.fillRect(displayX, displayY, 3, 3);

    gb.display.setColor(instruction == Instruction::Turn ? BEIGE : WHITE);
    if (instruction ==  Instruction::Turn) {
      gb.display.drawPixel(displayX, displayY);
    } else {
      gb.display.fillRect(displayX, displayY, 2, 2);
    }
  }
}

void drawProgram(Computer& computer) {
  for (int x = 0; x < computer.getSize(); x++) {
    int x0 = getDisplayX(x) + 1;
    for (int y = 0; y < computer.getSize(); y++) {
      int y0 = getDisplayY(y) + 1;
      drawInstruction(x0, y0, computer.getInstruction(x, y));
    }
  }
}

void drawButton(const char* label, bool selected) {
  int x = board_x0 + 2;
  int y = board_y0 + computer.getSize() * 5 + 2;
  int w = computer.getSize() * 5 - 4;

  gb.display.setColor(selected ? BLUE : DARKBLUE);
  gb.display.fillRoundRect(x, y, w, 7, 2);

  gb.display.setColor(BLACK);
  gb.display.setCursor(40 - strlen(label) * 2, y + 1);
  gb.display.print(label);
}

struct CountBucket {
  int minRange;
  int maxRange;
  uint8_t count;
  uint8_t next;
};

const uint8_t maxBuckets = (maxProgramSize - 1) * (maxProgramSize - 1);
const uint8_t EOL = maxBuckets; // Signals End of List

uint8_t numBuckets = 0;
uint8_t bucketListHeadIndex;
CountBucket buckets[maxBuckets];

void updateOrCreateBucketForVisit(int visits) {
  int bucketIndex = 0;

  // Find bucket for this visit count
  while (bucketIndex < numBuckets && buckets[bucketIndex].minRange != visits) {
    bucketIndex++;
  }

  CountBucket& bucket = buckets[bucketIndex];
  if (bucketIndex < numBuckets) {
    bucket.count++;
  } else {
    // New bucket
    bucket.minRange = visits;
    bucket.maxRange = visits;
    bucket.count = 1;
    numBuckets++;

    // Insert into sorted linked list
    uint8_t nextBucketIndex = bucketListHeadIndex;
    uint8_t prevBucketIndex = 0;
    while (nextBucketIndex != EOL && buckets[nextBucketIndex].minRange < visits) {
      prevBucketIndex = nextBucketIndex;
      nextBucketIndex = buckets[nextBucketIndex].next;
    }
    bucket.next = nextBucketIndex;
    if (nextBucketIndex == bucketListHeadIndex) {
      bucketListHeadIndex = bucketIndex;
    } else {
      buckets[prevBucketIndex].next = bucketIndex;
    }
  }
}

int getVisitCount(Computer& computer, int x, int y, bool horizontal) {
  if (horizontal) {
    return (
      computer.getExitCount(x, y, Direction::Right) +
      computer.getExitCount(x + 1, y, Direction::Left)
    );
  } else {
    return (
      computer.getExitCount(x, y, Direction::Up) +
      computer.getExitCount(x, y + 1, Direction::Down)
    );
  }
}

void emptyBuckets() {
  numBuckets = 0;
  bucketListHeadIndex = EOL;
}

void fillVisitBuckets(Computer& computer, bool horizontal) {
  for (int x = 0; x < computer.getSize() - 1; x++) {
    for (int y = 0; y < computer.getSize() - 1; y++) {
      int visits = getVisitCount(computer, x, y, horizontal);
      if (visits > 0) {
        updateOrCreateBucketForVisit(visits);
      }
    }
  }
}

void collapseVisitBuckets(int targetNum) {
  while (numBuckets > targetNum) {
    uint8_t bucketIndex = bucketListHeadIndex;
    uint32_t minArea = 0;
    uint8_t minBucketIndex = 0;

    // Find bucket to collapse
    while (buckets[bucketIndex].next != EOL) {
      uint8_t nextBucketIndex = buckets[bucketIndex].next;
      uint32_t collapsedArea = (
        buckets[bucketIndex].count +
        buckets[nextBucketIndex].count
      ) * 0 + (
        buckets[nextBucketIndex].maxRange -
        buckets[bucketIndex].minRange
      );
      if (minArea == 0 || collapsedArea < minArea) {
        minArea = collapsedArea;
        minBucketIndex = bucketIndex;
      }
      bucketIndex = nextBucketIndex;
    }

    // Collapse bucket
    uint8_t oldNextIndex = buckets[minBucketIndex].next;
    buckets[minBucketIndex].next = buckets[oldNextIndex].next;
    buckets[minBucketIndex].count += buckets[oldNextIndex].count;
    buckets[minBucketIndex].maxRange = buckets[oldNextIndex].maxRange;
    numBuckets--;
  }
}

const int numVisitColors = 5;
Color visitColors[numVisitColors] = {
  BLUE, GREEN, LIGHTBLUE, LIGHTGREEN, YELLOW
};

Color getColorForVisitCount(int count) {
  uint8_t nextBucketIndex = buckets[bucketListHeadIndex].next;
  uint8_t colorIndex = 0;
  while (nextBucketIndex != EOL && buckets[nextBucketIndex].minRange <= count) {
    colorIndex++;
    nextBucketIndex = buckets[nextBucketIndex].next;
  }
  return visitColors[colorIndex];
}

void dumpBuckets() {
  uint8_t bucketIndex = bucketListHeadIndex;
  while (bucketIndex != EOL) {
    SerialUSB.printf("%d-%d (%d) ", buckets[bucketIndex].minRange, buckets[bucketIndex].maxRange, buckets[bucketIndex].count);
    bucketIndex = buckets[bucketIndex].next;
  }
  SerialUSB.printf("\n");
}

void drawVisitCounts(Computer& computer, bool horizontal) {
  for (int x = 0; x < computer.getSize() - 1; x++) {
    for (int y = 0; y < computer.getSize() - 1; y++) {
      int visits = getVisitCount(computer, x, y, horizontal);
      if (visits > 0) {
        int x0 = getDisplayX(x);
        int y0 = getDisplayY(y);
        gb.display.setColor(getColorForVisitCount(visits));
        if (horizontal) {
          gb.display.drawLine(x0 + 2, y0 + 2, x0 + 7, y0 + 2);
        } else {
          gb.display.drawLine(x0 + 2, y0 + 2, x0 + 2, y0 - 3);
        }
      }
    }
  }
}

void drawVisitCounts(Computer& computer) {
  emptyBuckets();
  fillVisitBuckets(computer, true);
  fillVisitBuckets(computer, false);

  collapseVisitBuckets(numVisitColors);

  drawVisitCounts(computer, true);
  drawVisitCounts(computer, false);
}

void drawRunStatus(Computer& computer) {
  gb.display.setCursorX(48);
  gb.display.setCursorY(0);
  if (computer.getStatus() != Status::Ready) {
    gb.display.setColor(
      computer.getStatus() == Status::Running ? BLUE : (
        computer.getStatus() == Status::Done ? DARKBLUE : RED)
    );
    gb.display.printf("%8d", computer.getNumSteps());
  }

  gb.display.setCursorX(0);
  gb.display.setCursorY(0);
  if (computer.getStatus() == Status::Error) {
    gb.display.setColor(RED);
    gb.display.printf("Error!");
  }
}

