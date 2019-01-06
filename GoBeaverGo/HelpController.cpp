#include "HelpController.h"

#include <Gamebuino-Meta.h>

#include "Globals.h"
#include "Drawing.h"
#include "MainMenuController.h"

void HelpController::update() {
  if (gb.buttons.pressed(BUTTON_A)) {
    setController(&mainMenuController);
  }
}

void HelpController::draw() {
  gb.display.setColor(BLUE);

  gb.display.drawImage(10, 0, goBeaverGoImage);

  gb.display.setCursor(0, 11);
  gb.display.print("Edit: Use A and B to");
  gb.display.setCursor(0, 18);
  gb.display.print("place   and   stones");

  gb.display.setCursor(0, 31);
  gb.display.print("Run: Use A to Step,");
  gb.display.setCursor(0, 38);
  gb.display.print("Play, Pause & Reset");

  gb.display.setCursor(0, 51);
  gb.display.print("Use Menu to switch");
  gb.display.setCursor(0, 58);
  gb.display.print("between Edit and Run");

  gb.display.setColor(LIGHTBLUE);
  gb.display.setCursor(0, 11);
  gb.display.print("Edit");
  gb.display.setCursor(0, 31);
  gb.display.print("Run");
  gb.display.setCursor(32, 58);
  gb.display.print("Edit");
  gb.display.setCursor(68, 58);
  gb.display.print("Run");

  gb.display.setColor(WHITE);
  gb.display.setCursor(40, 11);
  gb.display.print("A");
  gb.display.setCursor(64, 11);
  gb.display.print("B");
  gb.display.setCursor(36, 31);
  gb.display.print("A");
  gb.display.setCursor(16, 51);
  gb.display.print("Menu");

  drawInstruction(24, 19, Instruction::Turn);
  drawInstruction(48, 19, Instruction::Data);
}

