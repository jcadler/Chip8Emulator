#include <cstdint>
#include <GL/freeglut.h>
#include <unordered_map>
#include <thread>

#include "beep.h"
#include "core.h"
#include "inst.h"
#include "screen.h"
#include "timer.h"

class Core_impl
  : public Core
{
  InstructionParser * instParser;

  void start_beep()
  {
    beep_play();
  }

  void stop_beep()
  {
    beep_pause();
  }

public:
  Core_impl()
  {
    disp = initWindow();
  }

  ~Core_impl()
  {
    delete disp;
  }

  void run()
  {
    int x_prev = x_global;
    int y_prev = y_global;
    disp->set_pixel_wrap(x_global, y_global);
    beep_play();
    sound_register = 60 * 4;
    while(1) {
      if( x_global != x_prev || y_global != y_prev ) {
	disp->clear_pixel_wrap(x_prev, y_prev);
	disp->set_pixel_wrap(x_global, y_global);
	x_prev = x_global;
	y_prev = y_global;
      }

      // instruction load
      uint8_t instruction_msb = ram[PC];
      uint8_t instruction_lsb = ram[PC+1];
      uint16_t instruction = 0;
      instruction &= instruction_msb;
      instruction << 8;
      instruction &= instruction_lsb;
      
      
      // update display
      glutMainLoopEvent();
      disp->render();

      // update timers
      if( delay_register != 0 ) {
	delay_register -= delay_timer.tick();
      }
      if( sound_register != 0 ) {
	sound_register -= sound_timer.tick();
	if( sound_register == 0 ) {
	  beep_pause();
	}
      }
    }
  }
};

Core * initCore()
{
  return new Core_impl();
}
