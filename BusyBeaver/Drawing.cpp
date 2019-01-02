#include <Gamebuino-Meta.h>

#include "Drawing.h"

#include "Program.h"
#include "RunController.h"

const int board_x0 = 17;
const int board_y0 = 7;

struct SpeedBarSpecs {
  Color color;
  int len;
};

const SpeedBarSpecs speedBarSpecs[5] = {
  SpeedBarSpecs { .color = RED, .len = 1 },
  SpeedBarSpecs { .color = ORANGE, .len = 5 },
  SpeedBarSpecs { .color = YELLOW, .len = 5 },
  SpeedBarSpecs { .color = GREEN, .len = 5 },
  SpeedBarSpecs { .color = LIGHTGREEN, .len = 5 }
};

int getDisplayX(int addressX) {
  return board_x0 + 5 * addressX;
}

int getDisplayY(int addressY) {
  return board_y0 + 5 * (8 - addressY);
}

void drawSpeedBar(int speed) {
  gb.display.setColor(DARKGRAY);
  gb.display.drawRect(0, 8, 6, (maxRunSpeed + 2) * 2);

  int spec = 0;
  int num = 0;

  for (int i = 0; i <= speed; i++) {
    if (num == 0) {
      gb.display.setColor(speedBarSpecs[spec].color);
    }
    gb.display.fillRect(1, 9 + (maxRunSpeed - i) * 2, 4, 2);
    if (++num == speedBarSpecs[spec].len) {
      num = 0;
      spec++;
    }
  }
}

void drawCursor(int x, int y) {
  gb.display.setColor(BLUE);
  gb.display.drawRect(getDisplayX(x), getDisplayY(y), 5, 5);
}

void drawProgramPointer(Program& program) {
  int x0 = getDisplayX(program.getAddressX());
  int y0 = getDisplayY(program.getAddressY());

  gb.display.setColor(LIGHTGREEN);
  gb.display.drawRect(x0, y0, 5, 5);

  gb.display.setColor(YELLOW);
  switch (program.getDirection()) {
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

void drawMemory(Program& program) {
  int p = min(0, program.getMemoryAddress());
  int x = 0;

  gb.display.setCursorY(59);
  while (x < 80) {
    gb.display.setCursorX(x);
    gb.display.setColor(p == program.getMemoryAddress() ? LIGHTGREEN : GREEN);

    if (p >= 0 && p < memorySize) {
      int val = program.getMemory(p);
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

void drawProgram(Program& program) {
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

  // Instructions
  for (int x = 0; x < program.getSize(); x++) {
    for (int y = 0; y < program.getSize(); y++) {
      Instruction ins = program.getInstruction(x, y);
      if (ins != Instruction::Nop) {
        if (ins == Instruction::Blk) {
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

const uint8_t maxBuckets = 1 + maxProgramSize * maxProgramSize;
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

void fillVisitBuckets(Program& program) {
  numBuckets = 0;
  bucketListHeadIndex = EOL;

  for (int x = 0; x < program.getSize(); x++) {
    for (int y = 0; y < program.getSize(); y++) {
      int visits = program.getVisitCount(x, y);
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
      ) * (
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
  LIGHTBLUE, BLUE, PURPLE, PINK, RED
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

void drawVisitCounts(Program& program) {
  fillVisitBuckets(program);
  dumpBuckets();
  collapseVisitBuckets(numVisitColors);
  dumpBuckets();

  for (int x = 0; x < program.getSize(); x++) {
    for (int y = 0; y < program.getSize(); y++) {
      int visits = program.getVisitCount(x, y);
      if (visits > 0) {
        int x0 = getDisplayX(x);
        int y0 = getDisplayY(y);
        gb.display.setColor(getColorForVisitCount(visits));
        //gb.display.drawPixel(x0 + 1, y0 + 1);
        //gb.display.drawPixel(x0 + 3, y0 + 1);
        //gb.display.drawPixel(x0 + 1, y0 + 3);
        //gb.display.drawPixel(x0 + 3, y0 + 3);
        gb.display.drawLine(x0 + 1, y0 + 2, x0 + 3, y0 + 2);
        gb.display.drawLine(x0 + 2, y0 + 1, x0 + 2, y0 + 3);
      }
    }
  }
}

void drawRunStatus(Program& program) {
  gb.display.setCursorX(48);
  gb.display.setCursorY(0);
  if (program.getStatus() == Status::Running || program.getStatus() == Status::Done) {
    gb.display.setColor(program.getStatus() == Status::Running ? BLUE : DARKBLUE);
    gb.display.printf("%8d", program.getNumSteps());
  }
  if (program.getStatus() == Status::Error) {
    gb.display.setColor(RED);
    gb.display.printf("Error!");
  }
}

