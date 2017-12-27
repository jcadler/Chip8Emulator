#pragma once

#include <cassert>
#include <GL/freeglut.h>

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640

#define WINDOW_PIXEL_HEIGHT 32
#define WINDOW_PIXEL_WIDTH 64

struct screen
{
  unsigned width;
  unsigned height;
  float width_pct;
  float height_pct;
  bool ** pixels;

  screen(unsigned w, unsigned h)
  {
    width = w;
    height = h;
    width_pct = 2.0 / width;
    height_pct = 2.0 / height;
    pixels = new bool*[width];
    for( int i=0; i < width; i++ )
      {
	pixels[i] = new bool[height];
	for( int j=0; j < height; j++ )
	  {
	    pixels[i][j]=false;
	  }
      }
  }

public:
  void set_pixel(unsigned x, unsigned y)
  {
    assert( x < width && y < height);
    pixels[x][y] = true; 
  }

  void clear_pixel(unsigned x, unsigned y)
  {
    assert( x < width && y < height);
    pixels[x][y] = false; 
  }

  void set_pixel_wrap(int x, int y)
  {
    set_pixel(abs(x % width), abs(y % height));
  }

  void clear_pixel_wrap(int x, int y)
  {
    clear_pixel(abs(x % width), abs(y % height));
  }

  void render(void)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0; i < width; i++)
      {
	for(int j=0; j < height; j++)
	  {
	    float top_x = width_pct*i-1;
	    float top_y = 1-height_pct*j;
	    float bot_x = top_x + width_pct;
	    float bot_y = top_y - height_pct;
	    if(pixels[i][j])
	      {
		glRectf(top_x, top_y,
			bot_x, bot_y);
	      }
	  }
      }
  
    glFlush();
  }

};

int y_global = 15;
int x_global = 15;

void processKey(unsigned char key, int x, int y)
{
  switch(key) {
  case 't':
    y_global--;
    break;
  case 'g':
    y_global++;
    break;
  case 'f':
    x_global--;
    break;
  case 'h':
    x_global++;
    break;
  default:
    break;
  }
}

screen * initWindow()
{
  // glut doesn't let you call glutInit with no args,
  // so these false-argc and argv params are necessary.
  // doesn't feel good, but it works
  int argc = 1;
  char *argv[1] = {(char*)""};
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
  glutCreateWindow("CHIP-8 Interpreter");
  glutKeyboardFunc(processKey);
  glutKeyboardUpFunc(NULL);

  return new screen(WINDOW_PIXEL_WIDTH, WINDOW_PIXEL_HEIGHT);
}
