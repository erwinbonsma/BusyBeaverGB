#include <Gamebuino-Meta.h>

class Computer;

void drawProgramSpace();
void drawProgram(Computer& computer);
void drawMemory(Computer& computer);
void drawRunStatus(Computer& computer);
void drawCursor(int x, int y);
void drawProgramPointer(Computer& computer);
void drawVisitCounts(Computer& computer);

void drawSpeedBar(int speed);
