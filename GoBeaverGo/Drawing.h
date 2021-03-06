/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#include <Gamebuino-Meta.h>

#include "Computer.h"

int getDisplayX(int addressX);
int getDisplayY(int addressY);

void drawInstruction(int displayX, int displayY, Instruction instruction);

void resetDrawing();

void drawProgramSpace();
void drawProgram(Computer& computer);
void drawData(Computer& computer, int centerAddress);
void drawRunStatus(Computer& computer);
void drawCursor(int x, int y);
void drawProgramPointer(Computer& computer);
void drawButton(const char* label, bool selected);
void drawVisitCounts(Computer& computer);

void drawSpeedBar(int speed);
