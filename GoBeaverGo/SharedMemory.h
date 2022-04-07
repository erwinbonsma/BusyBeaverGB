/*
 * Go Beaver Go, a puzzle game based on a variant of the 2L programming language
 *
 * Copyright 2022, Erwin Bonsma
 */

#ifndef __SHARED_MEMORY_INCLUDED
#define __SHARED_MEMORY_INCLUDED

#include "Computer.h"
#include "Store.h"

struct ExecutionMemory {
  int16_t data[dataSize];
};

union SharedMemory {
  ExecutionMemory execution;
  StorageManagement storage;
};

extern SharedMemory shared_mem;

#endif
