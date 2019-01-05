#include <Gamebuino-Meta.h>

#include "Drawing.h"

#include "Computer.h"
#include "RunController.h"

const int board_x0 = 17;
const int board_y0 = 7;

int getDisplayX(int addressX) {
  return board_x0 + 5 * addressX;
}

int getDisplayY(int addressY) {
  return board_y0 + 5 * (8 - addressY);
}

void drawSpeedBar(int speed) {
  gb.display.setColor(DARKGRAY);
  gb.display.drawRect(0, 8, 5, (maxRunSpeed + 2) * 2);

  gb.display.setColor(GRAY);
  gb.display.fillRect(1, 9 + (maxRunSpeed - speed) * 2, 3, (speed + 1) * 2);
}

void drawCursor(int x, int y) {
  gb.display.setColor(BLUE);
  gb.display.drawRect(getDisplayX(x), getDisplayY(y), 5, 5);
}

void drawProgramPointer(Computer& computer) {
  ProgramPointer pp = computer.getProgramPointer();
  int x0 = getDisplayX(pp.x);
  int y0 = getDisplayY(pp.y);

  gb.display.setColor(LIGHTGREEN);
  gb.display.drawRect(x0, y0, 5, 5);

  gb.display.setColor(YELLOW);
  switch (pp.dir) {
    case Direction::Up:
      gb.display.drawLine(x0, y0, x0 + 4, y0);
      break;
    case Direction::Down:
      gb.display.drawLine(x0, y0 + 4, x0 + 4, y0 + 4);
      break;
    case Direction::Left:
      gb.display.drawLine(x0, y0, x0, y0 + 4);
      break;
    case Direction::Right:
      gb.display.drawLine(x0 + 4, y0, x0 + 4, y0 + 4);
      break;
  }
}

void drawData(Computer& computer) {
  int p = 0;

  // Move to part of the tape containing non-zero data and/or data pointer
  while (
    p < dataSize &&
    p < computer.getDataPointer() &&
    computer.getData(p) == 0
  ) {
    p++;
  }

  if (computer.getDataPointer() - p > 8) {
    p = computer.getDataPointer() - 8;
  }

  int x = 0;
  gb.display.setCursorY(59);
  while (x < 80) {
    gb.display.setCursorX(x);
    gb.display.setColor(p == computer.getDataPointer() ? LIGHTGREEN : GREEN);

    if (p >= 0 && p < dataSize) {
      int val = computer.getData(p);
      x += gb.display.print(val, DEC) * 4;
    }
    else {
      x += gb.display.print("*") * 4;
    }

    p++;
    // Using intermediate x variable instead of setCursorX to avoid termination
    // problems (due to auto-carriage return?).
    x += 1;
  }
}

void drawProgramSpace() {
  // Board
  gb.display.setColor(BROWN);
  gb.display.fillRect(board_x0, board_y0, 45, 45);

  // Grid lines
  gb.display.setColor(DARKGRAY);
  for (int i = 0; i < 9; i++) {
    gb.display.drawLine(
      board_x0 + 2 + 5 * i, board_y0 +  2,
      board_x0 + 2 + 5 * i, board_y0 + 41
    );
    gb.display.drawLine(
      board_x0 +  2, board_y0 + 2 + 5 * i,
      board_x0 + 41, board_y0 + 2 + 5 * i
    );
  }
}

void drawProgram(Computer& computer) {
  // Instructions
  for (int x = 0; x < computer.getSize(); x++) {
    for (int y = 0; y < computer.getSize(); y++) {
      Instruction ins = computer.getInstruction(x, y);
      if (ins != Instruction::Noop) {
        if (ins == Instruction::Turn) {
          gb.display.setColor(BLACK);
        } else {
          gb.display.setColor(WHITE);
        }
        gb.display.fillRect(board_x0 + 1 + 5 * x, board_y0 + 1 + 5 * (8 - y), 3, 3);
      }
    }
  }
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

const int numVisitColors = 8;
Color visitColors[numVisitColors] = {
  LIGHTBLUE, BLUE, DARKBLUE, GREEN, LIGHTGREEN, YELLOW, ORANGE, RED
};

Color getColorForVisitCount(int count) {
  uint8_t nextBucketIndex = buckets[bucketListHeadIndex].next;
  uint8_t colorIndex = 0;
  while (nextBucketIndex != EOL && buckets[nextBucketIndex].minRange <= count) {
    colorIndex++;
    nextBucketIndex = buckets[nextBucketIndex].next;
  }
  //SerialUSB.printf("%d => %d, ", count, colorIndex);
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

