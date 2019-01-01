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
  int x0 = board_x0 + 5 * x;
  int y0 = board_y0 + 5 * (8 - y);

  gb.display.setColor(BLUE);
  gb.display.drawRect(x0, y0, 5, 5);
}

void drawProgramPointer(Program& program) {
  int x0 = board_x0 + 5 * program.getAddressX();
  int y0 = board_y0 + 5 * (8 - program.getAddressY());

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

void drawRunStatus(Program& program) {
  gb.display.setCursorX(56);
  gb.display.setCursorY(0);
  if (program.getStatus() == Status::Running || program.getStatus() == Status::Done) {
    gb.display.setColor(program.getStatus() == Status::Running ? BLUE : DARKBLUE);
    gb.display.printf("%6d", program.getNumSteps());
  }
  if (program.getStatus() == Status::Error) {
    gb.display.setColor(RED);
    gb.display.printf("Error!");
  }
}

