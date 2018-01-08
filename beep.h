#include <math.h>
#include <iostream>
#include <SFML/Audio.hpp>

struct sinBeep
{
  sf::Int16 * samples;
  int count;
  sf::SoundBuffer buff;
  sf::Sound beep;
  bool fail = false;

  sinBeep(int sample_count, int freq, int amp)
    : count(sample_count)
  {
    samples = new sf::Int16[count];
    double increment = (double)freq / count;
    double x = 0;
    for( int i = 0; i < count; i++ ) {
      samples[i] = amp * sin(x * M_PI * 2);
      x += increment;
    }
    if( !buff.loadFromSamples(samples, count, 1, count) ) {
      printf("Failed to load samples!\n");
      fail = true;
    }
    beep.setBuffer(buff);
    beep.setLoop(true);
  }

public:
  void play()
  {
    printf("hello there\n");
    beep.play();
  }

  void pause()
  {
    printf("general kenobi\n");
    beep.pause();
  }
};

sinBeep beeper(44100, 440, 30000);

void beep_play()
{
  beeper.play();
}

void beep_pause()
{
  beeper.pause();
}
