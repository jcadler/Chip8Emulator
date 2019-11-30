#include <assert.h>
#include <stdlib.h>
#include <vector>

#include "core.h"
#include "inst.h"



class Clear_Screen
  : public Instruction
{
public:
  Clear_Screen()
    : Instruction(0xFFFF, 0x00E0)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned height = core.disp->get_pixel_height();
    unsigned width = core.disp->get_pixel_width();
    for (unsigned x = 0; x < height; x++) {
      for (unsigned y = 0; y < width; y++) {
	core.disp->clear_pixel(x, y);
      }
    }
  }
};

class Ret
  : public Instruction
{
public:
  Ret()
    : Instruction(0xFFFF, 0x00EE)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    core.PC = core.stack[core.SP];
    core.SP--;
  }
};

class Jump
  : public Instruction
{
public:
  Jump()
    : Instruction(0xF000, 0x1000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    core.PC = inst & 0x0FFF;
  }
};

class Call
  : public Instruction
{
public:
  Call()
    : Instruction(0xF000, 0x2000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    core.SP++;
    core.stack[core.SP] = core.PC;
    core.PC = inst & 0x0FFF;
  }
};

class Skip_Compare_Val
  : public Instruction
{
public:
  Skip_Compare_Val()
    : Instruction(0xF000, 0x3000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg = extract_reg1(inst);
    uint8_t comp_val = inst & 0x00FF;
    if (comp_val == core.registers[reg]) {
      core.PC++;
    }
  }
};

class Skip_Compare_Not_Val
  : public Instruction
{
public:
  Skip_Compare_Not_Val()
    : Instruction(0xF000, 0x4000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg = extract_reg1(inst);
    uint8_t comp_val = inst & 0x00FF;
    if (comp_val != core.registers[reg]) {
      core.PC++;
    }
  }
};

class Skip_Compare_Reg
  : public Instruction
{
public:
  Skip_Compare_Reg()
    : Instruction(0xF00F, 0x5000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    if (core.registers[reg1] == core.registers[reg2]) {
      core.PC++;
    }
  }
};

class Load_Register_Val
  : public Instruction
{
public:
  Load_Register_Val()
    : Instruction(0xF000, 0x6000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg = extract_reg1(inst);
    uint8_t val = inst & 0x00FF;
    core.registers[reg] = val;
  }
};

class Add_Val
  : public Instruction
{
public:
  Add_Val()
    : Instruction(0xF000, 0x7000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg = extract_reg1(inst);
    uint8_t val = inst & 0x00FF;
    core.registers[reg] += val;
  }
};

class Load_Register_Register
  : public Instruction
{
public:
  Load_Register_Register()
    : Instruction(0xF00F, 0x8000)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    core.registers[reg1] = core.registers[reg1];
  }
};

class Or
  : public Instruction
{
public:
  Or()
    : Instruction(0xF00F, 0x8001)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    core.registers[reg1] = core.registers[reg1] | core.registers[reg2];
  }
};

class And
  : public Instruction
{
public:
  And()
    : Instruction(0xF00F, 0x8002)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    core.registers[reg1] = core.registers[reg1] & core.registers[reg2];
  }
};

class Xor
  : public Instruction
{
public:
  Xor()
    : Instruction(0xF00F, 0x8003)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    core.registers[reg1] = core.registers[reg1] ^ core.registers[reg2];
  }
};

class Add_Register
  : public Instruction
{
public:
  Add_Register()
    : Instruction(0xF00F, 0x8004)
  {}
  
protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    core.registers[reg1] += core.registers[reg2];
    core.registers[15] = core.registers[reg1] < core.registers[reg2] ? 1 : 0;
  }
};

class Sub_Register
  : public Instruction
{
public:
  Sub_Register()
    : Instruction(0xF00F, 0x8004)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    core.registers[15] = core.registers[reg1] > core.registers[reg2] ? 1 : 0;
    core.registers[reg1] -= core.registers[reg2];
  }
};

class Shift_Right
  : public Instruction
{
public:
  Shift_Right()
    : Instruction(0xF00F, 0x8006)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1 = extract_reg1(inst);
    core.registers[15] = core.registers[reg1] & 0x0001;
    core.registers[reg1] = core.registers[reg1] >> 1;
  }
};

class Sub_Register_Reverse
  : public Instruction
{
public:
  Sub_Register_Reverse()
    : Instruction(0xF00F, 0x8007)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    core.registers[15] = core.registers[reg2] > core.registers[reg1] ? 1 : 0;
    core.registers[reg1] = core.registers[reg2] - core.registers[reg1];
  }
};

class Shift_Left
  : public Instruction
{
public:
  Shift_Left()
    : Instruction(0xF00F, 0x800E)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1 = extract_reg1(inst);
    core.registers[15] = core.registers[reg1] & 0x8000 ? 1 : 0;
    core.registers[reg1] = core.registers[reg1] << 1;
  }
};

class Skip_Compare_Not_Register
  : public Instruction
{
public:
  Skip_Compare_Not_Register()
    : Instruction(0xF00F, 0x9000)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1, reg2;
    extract_reg1_reg2(inst, &reg1, &reg2);
    if (core.registers[reg1] != core.registers[reg2]) {
      core.PC++;
    }
  }
};

class Load_I
  : public Instruction
{
public:
  Load_I()
    : Instruction(0xF000, 0xA000)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    core.I = inst & 0x0FFF;    
  }
};

class Jump_Add_Register
  : public Instruction
{
public:
  Jump_Add_Register()
    : Instruction(0xF000, 0xB000)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    uin16_t addr = inst & 0x0FFF;
    core.PC = addr + core.registers[0];
  }
};

class Random_Generate_And
  : public Instruction
{
public:
  Random_Generate()
    : Instruction(0xF000, 0xC000)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {
    unsigned reg1 = extract_reg1(inst);
    uin8_t and_val = inst & 0x00FF;
    core.registers[reg1] = (rand() % 255) & and_val;
  }
};

class 
  : public Instruction
{
public:
  ()
    : Instruction(0x, 0x)
  {}

protected:
  void visit_f(Core & core, uint16_t inst) override
  {

  }
};

class Instruction_Processor_impl
  : public Instruction_Processor
{
  Core & core;
  std::vector<Instruction> instructions;
  
public:
  Instruction_Processor_impl(Core & core_)
    : core(core_)
  {}
  
  void process(uint16_t inst) override
  {
    for (auto & instruction : instructions) {
      instruction.visit(core, inst);
    }
  }
};

Instruction_Processor * initProcessor(Core & core)
{
  return new Instruction_Processor_impl(core);
}
