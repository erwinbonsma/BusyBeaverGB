#include <Gamebuino-Meta.h>

#include "Drawing.h"

#include "Program.h"

const int board_x0 = 17;
const int board_y0 = 5;

void drawPointer(Program& program) {
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
  int p = 0;

  gb.display.setCursorX(0);
  gb.display.setCursorY(59);
  while (gb.display.getCursorX() < 80) {
    gb.display.setColor(p == program.getMemoryAddress() ? LIGHTGREEN : GREEN);

    int val = program.getMemory(p++);
    gb.display.print(val, DEC);
    gb.display.setCursorX(gb.display.getCursorX() + 4);
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

  drawPointer(program);

  // Memory
  drawMemory(program);

  if (program.getNumSteps() > 0) {
    gb.display.setColor(GREEN);
    gb.display.setCursorX(0);
    gb.display.setCursorY(52);
    gb.display.printf("%d", program.getNumSteps());
  }
}

