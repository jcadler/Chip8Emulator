#include "screen.h"

struct Core {
  screen * disp;

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
    }
  }
};
