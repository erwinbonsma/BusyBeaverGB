class Computer;

class Challenge {
public:
  virtual void draw() = 0;
  virtual bool isAchieved(Computer& computer) = 0;
};

