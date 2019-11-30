#pragma once
#include <assert.h>

#include "core.h"
class Instruction
{  
 public:
  Instruction(uint16_t compare_mask_, uint16_t compare_val_)
    : compare_mask(compare_mask_)
    , compare_val(compare_val_)
  {}
  
  virtual void visit(Core & core, uint16_t inst)
  {
    if (visit_check(inst)) {
      visit_f(core, inst);
    }
  }

 protected:
  // defaults set to guarantee to fail the check
  uint16_t compare_mask;
  uint16_t compare_val;
  bool visit_check(uint16_t inst)
  {
    return (inst & compare_mask) == compare_val;
  }

  virtual void visit_f(Core & core, uint16_t inst) = 0;
  
  unsigned extract_reg1(uint16_t inst)
  {
    unsigned reg = (inst & 0x0F00) >> 2;
    assert (reg < 16);
    return reg;
  }

  unsigned extract_reg2(uint16_t inst)
  {
    unsigned reg = (inst & 0x00F0) >> 1;
    assert (reg < 16);
    return reg;
  }

  void extract_reg1_reg2(uint16_t inst, unsigned * reg1_out, unsigned * reg2_out)
  {
    *reg1_out = extract_reg1(inst);
    *reg2_out = extract_reg2(inst);
    return;
  }
};

class Instruction_Processor
{
 public:
  virtual void process(uint16_t inst) = 0;
};

Instruction_Processor * initProcessor(Core & core);
