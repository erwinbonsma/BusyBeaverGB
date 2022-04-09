/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2019, Erwin Bonsma
 */

#ifndef __STATS_CONTROLLER_INCLUDED
#define __STATS_CONTROLLER_INCLUDED

#include "Controller.h"

struct StatsMemory {
  int min_value;
  int max_value;
  int max_run_len;
  int max_seq_len;
  int num_completed_tutorials;
  int num_completed_challenges;
};

class StatsController : public Controller {
public:
  void activate();
  void update();
  void draw();
};

#endif
