/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

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
  gb.display.print("program with   and");
  gb.display.setCursor(0, 25);
  gb.display.print("Select Run when done");

  gb.display.setCursor(0, 41);
  gb.display.print("Run: Use A to pause,");
  gb.display.setCursor(0, 48);
  gb.display.print("play and step. Use B");
  gb.display.setCursor(0, 55);
  gb.display.print("to return to editing");

  gb.display.setColor(LIGHTBLUE);
  gb.display.setCursor(0, 11);
  gb.display.print("Edit");
  gb.display.setCursor(0, 41);
  gb.display.print("Run");

  gb.display.setColor(WHITE);
  gb.display.setCursor(40, 11);
  gb.display.print("A");
  gb.display.setCursor(64, 11);
  gb.display.print("B");
  gb.display.setCursor(28, 25);
  gb.display.print("Run");
  gb.display.setCursor(36, 41);
  gb.display.print("A");
  gb.display.setCursor(76, 48);
  gb.display.print("B");

  drawInstruction(52, 19, Instruction::Turn);
  drawInstruction(76, 19, Instruction::Data);
}

