#include <cstdint>
#include <thread>

#include "beep.h"
#include "screen.h"
#include "timer.h"

struct Core {
  screen * disp;
  uint8_t ram[4096];
  uint8_t registers[16];
  uint16_t I;
  uint8_t SP;  // stack pointer
  uint16_t PC; // program counter

  Timer<60> delay_timer;
  Timer<60> sound_timer;
  uint8_t delay_register = 0;
  uint8_t sound_register = 0;
  
  Core()
  {
    disp = initWindow();
  }

  ~Core()
  {
    delete disp;
  }

public:
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
