#ifndef __CONTROLLER_INCLUDED
#define __CONTROLLER_INCLUDED

class Controller {

public:
  virtual void activate() {};
  virtual void update() = 0;
  virtual void draw() = 0;
};

void setController(Controller* controller);

#endif
