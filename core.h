#include <cstdint>
#include <chrono>
#include <thread>

#include "screen.h"
#include "beep.h"

struct Core {
  screen * disp;
  uint8_t ram[4096];
  uint8_t registers[16];
  uint16_t I;
  uint8_t SP;  // stack pointer
  uint16_t PC; // program counter
  uint8_t delay_timer = 0;
  uint8_t sound_timer = 0;

  using timer_tick = std::chrono::duration< uint8_t, std::ratio<1, 60> >;
  using timer_start = std::chrono::time_point<std::chrono::steady_clock>;
  timer_start delay_start;
  timer_start sound_start;
  
  Core()
  {
    disp = initWindow();
  }

  ~Core()
  {
    delete disp;
  }

private:
  void set_delay_timer(uint8_t val)
  {
    delay_timer = val;
    delay_start = std::chrono::steady_clock::now();
  }

  void set_sound_timer(uint8_t val)
  {
     sound_timer = val;
     sound_start = std::chrono::steady_clock::now();
  }

public:
  void run()
  {
    int x_prev = x_global;
    int y_prev = y_global;
    disp->set_pixel_wrap(x_global, y_global);
    set_delay_timer(60 * 4);
    beep_play();
    bool timer_hit = false;
    while(1) {
      if( x_global != x_prev || y_global != y_prev ) {
	disp->clear_pixel_wrap(x_prev, y_prev);
	disp->set_pixel_wrap(x_global, y_global);
	x_prev = x_global;
	y_prev = y_global;
      } 
      std::this_thread::sleep_for(std::chrono::seconds(1)/60.0);
      glutMainLoopEvent();
      disp->render();
      if( delay_timer != 0 ) {
	uint8_t delay_ticks = std::chrono::duration_cast<timer_tick>(std::chrono::steady_clock::now() - delay_start).count();
	if( delay_ticks >= 1 ) {
	  delay_timer -= delay_ticks;
	  delay_start = std::chrono::steady_clock::now();
	}
      }
      if( !timer_hit && delay_timer == 0 ) {
	timer_hit = true;
	printf("4 seconds have passed!\n");
	beep_pause();
      }
    }
  }
};
