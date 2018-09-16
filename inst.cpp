#include <cassert>
#include <cstdint>
#include <unordered_map>

#include "core.h"
#include "inst.h"
#include "screen.h"

class ClearDisplay
  : public InstructionParser {
public:
  ClearDisplay(Core & _core)
    : InstructionParser(_core)
  {}

  void parse(uint16_t inst)
  {
    assert(inst == 0x00E0);
    for(unsigned x = 0; x < core.disp->get_pixel_width(); x++) {
      for(unsigned y = 0; y < core.disp->get_pixel_height(); y++) {
	core.disp->clear_pixel(x, y);
      }
    }
  }
};
  
class Ret
  : public InstructionParser {
public:
    Ret(Core & _core)
    : InstructionParser(_core)
  {}

  void parse(uint16_t inst)
  {
    assert(inst == 0x00EE);
    core.PC = core.ram[core.SP];
    core.SP--;
  }
};

class ZeroParser
  : public InstructionParser {

  std::unordered_map<uint16_t, InstructionParser> instructions;
public:
  ZeroParser(Core & _core)
    : InstructionParser(_core)
  {
    instructions.insert(std::make_pair<uint16_t, ClearDisplay>(0x00E0, core));
    instructions.insert(std::make_pair<uint16_t, Ret>(0x00EE, core));
  }

  void parse(uint16_t inst)
  {
    auto const & instruct = instructions.find(inst);
    if(instruct != instructions.end()) {
      instruct->second.parse(inst);
    }
  }
};

class Jump
  : public InstructionParser {
public:
  Jump(Core & _core)
    :InstructionParser(_core)
  {}

  void parse(uint16_t inst)
  {
    assert(inst & 0x1000);
    core.PC = get_address(inst);
  }
};
  
class BaseParser
  : public InstructionParser {
  std::unordered_map<uint8_t, InstructionParser> instructions;
public:
  BaseParser(Core & _core)
    : InstructionParser(_core)
    {
      instructions.insert(std::make_pair<uint8_t, ZeroParser>(0x0, core));
    }
  
  void parse(uint16_t inst)
  {
    auto const & instruct = instructions.find(inst >> 24);
    if(instruct != instructions.end()) {
      instruct->second.parse(inst);
    }
  }
};

InstructionParser * initInstParser(Core & core)
{
  return new BaseParser(core);
}
