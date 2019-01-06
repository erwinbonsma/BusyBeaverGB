class Computer;

void initSaveFileDefaults();

int getMaxCompletedChallenge();
void setMaxCompletedChallenge(int index);

bool storeProgram(Computer& computer);
bool loadProgram(Computer& computer);

