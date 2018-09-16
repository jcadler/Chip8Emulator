#pragma once

class Screen;

#include <cstdint>
#include "timer.h"

#define PROGRAM_START 0x200

class Core {
 public:
  Screen * disp;
  uint8_t ram[4096];
  uint8_t registers[16];
  uint16_t I;
  uint8_t SP;  // stack pointer
  uint16_t PC = PROGRAM_START; // program counter

  Timer<60> delay_timer;
  Timer<60> sound_timer;
  uint8_t delay_register = 0;
  uint8_t sound_register = 0;

  virtual void start_beep() = 0;
  virtual void stop_beep() = 0;

  virtual void run() = 0;
};

Core * initCore();
