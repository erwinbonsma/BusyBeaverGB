#include <Gamebuino-Meta.h>

#include "Drawing.h"

#include "Program.h"

const int board_x0 = 17;
const int board_y0 = 6;

void drawMemory(Program& program) {
  int p = 0;

  gb.display.setCursorX(0);
  gb.display.setCursorY(59);
  while (gb.display.getCursorX() < 80) {
    gb.display.setColor(p == program.getMemoryAddress() ? BEIGE : BROWN);

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

  // Pointer
  gb.display.setColor(RED);
  gb.display.drawRect(
    board_x0 + 5 * program.getAddressX(),
    board_y0 + 5 * (8 - program.getAddressY()),
    5, 5
  );

  // Memory
  drawMemory(program);
}

