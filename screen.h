#pragma once

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640

#define WINDOW_PIXEL_HEIGHT 32
#define WINDOW_PIXEL_WIDTH 64

struct Screen
{
  virtual void set_pixel(unsigned x, unsigned y) = 0;
  virtual void clear_pixel(unsigned x, unsigned y) = 0;
  virtual void set_pixel_wrap(int x, int y) = 0;
  virtual void clear_pixel_wrap(int x, int y) = 0;
  virtual void render() = 0;
};

Screen * initWindow();

extern int y_global;
extern int x_global;
