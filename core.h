#pragma once
#define PROGRAM_START 0x200

struct Core {
  virtual void run() = 0;
};

Core * initCore();
