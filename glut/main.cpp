#include <cassert>
#include <chrono>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <thread>

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640

#define WINDOW_PIXEL_HEIGHT 32
#define WINDOW_PIXEL_WIDTH 64

struct screen
{
  unsigned width;
  unsigned height;
  bool ** pixels;

  screen(unsigned w, unsigned h)
  {
    width = w;
    height = h;
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
};

screen scr(WINDOW_PIXEL_WIDTH, WINDOW_PIXEL_HEIGHT);

void render(void)
{
  float width_pct = 2.0/WINDOW_PIXEL_WIDTH;
  float height_pct = 2.0/WINDOW_PIXEL_HEIGHT;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(int i=0; i < WINDOW_PIXEL_WIDTH; i++)
    {
      for(int j=0; j < WINDOW_PIXEL_HEIGHT; j++)
	{
	  float top_x = width_pct*i-1;
	  float top_y = 1-height_pct*j;
	  float bot_x = top_x + width_pct;
	  float bot_y = top_y - height_pct;
	  if(scr.pixels[i][j])
	    {
	      glRectf(top_x, top_y,
		      bot_x, bot_y);
	    }
	}
    }
  
  glFlush();
}

int y_global = 15;
int x_global = 15;

void processKey(unsigned char key, int x, int y)
{
  printf("pressed the %c key\n", key);
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

void changeSize(int w, int h)
{
  if( w!=WINDOW_WIDTH || h!=WINDOW_HEIGHT)
    glutReshapeWindow(WINDOW_WIDTH,WINDOW_HEIGHT);
}

int main(int argc, char ** argv)
{
  printf("hello\n");
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
  glutCreateWindow("John's Window");
  glutKeyboardFunc(processKey);
  
  int x_prev = x_global;
  int y_prev = y_global;
  scr.set_pixel_wrap(x_global, y_global);
  while(1) {
    if( x_global != x_prev || y_global != y_prev ) {
      scr.clear_pixel_wrap(x_prev, y_prev);
      scr.set_pixel_wrap(x_global, y_global);
      x_prev = x_global;
      y_prev = y_global;
    } 
    x_prev = x_global;
    y_prev = y_global;
    std::this_thread::sleep_for(std::chrono::seconds(1)/60.0);
    glutMainLoopEvent();
    render();
  }
  return 0;
}
