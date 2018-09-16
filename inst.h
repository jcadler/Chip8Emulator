#pragma once

#include "core.h"

class InstructionParser {
 protected:
  Core & core;

  InstructionParser(Core & _core)
  : core(_core)
  {    
  }
 public:
  virtual void parse(uint16_t inst);

  static uint16_t get_address(uint16_t inst)
  {
    return inst & 0x0FFF;
  }

  static uint16_t get_arg(uint16_t inst, unsigned index)
  {
    return (inst >> (8 * index)) & 0x000F;
  }
};

InstructionParser * initInstParser(Core & core);
