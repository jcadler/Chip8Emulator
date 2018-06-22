#pragma once

template <unsigned int N>
struct Timer {
  using timer_tick = std::chrono::duration< int, std::ratio<1, N> >;
  using timer_start = std::chrono::time_point<std::chrono::steady_clock>;

  timer_start start;

  Timer()
  {
    start = std::chrono::steady_clock::now();;
  }


  int tick()
  {
    int ticks = std::chrono::duration_cast<timer_tick>( std::chrono::steady_clock::now() - start ).count();
    if( ticks > 0 ) { 
      start = std::chrono::steady_clock::now();
    }
    return ticks;
  }
};
