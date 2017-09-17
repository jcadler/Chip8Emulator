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
  void set_pixel(int x, int y)
  {
    assert( x < width && y < height);
    pixels[x][y] = true; 
  }

  void clear_pixel(int x, int y)
  {
    assert( x < width && y < height);
    pixels[x][y] = false; 
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

void changeSize(int w, int h)
{
  if( w!=WINDOW_WIDTH || h!=WINDOW_HEIGHT)
    glutReshapeWindow(WINDOW_WIDTH,WINDOW_HEIGHT);
}

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
  glutCreateWindow("John's Window");
  
  glutDisplayFunc(render);
  glutReshapeFunc(changeSize);
  scr.set_pixel(63,0);
  
  int x = 0;
  int y = 15;

  int x_prev = 0;
  while(1) {
    x_prev = x;
    if( ++x >= scr.width ) {
      x = 0;
    }
    scr.set_pixel(x, y);
    scr.clear_pixel(x_prev, y);
    std::this_thread::sleep_for(std::chrono::seconds(1)/2.0);
    glutMainLoopEvent();
    render();
  }
  return 0;
}
