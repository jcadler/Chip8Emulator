#include <cassert>
#include <GL/freeglut.h>

#include "screen.h"

struct Screen_impl
  : Screen
{
  unsigned width;
  unsigned height;
  float width_pct;
  float height_pct;
  bool ** pixels;

public:
  Screen_impl(unsigned w, unsigned h)
  {
    width = w;
    height = h;
    width_pct = 2.0 / width;
    height_pct = 2.0 / height;
    pixels = new bool*[width];
    for(unsigned i = 0; i < width; i++)
      {
	pixels[i] = new bool[height];
	for(unsigned j = 0; j < height; j++)
	  {
	    pixels[i][j]=false;
	  }
      }
  }

  ~Screen_impl()
  {
    for(unsigned x = 0; x < width; x++)
      {
	delete pixels[x];
      }
    delete pixels;
  }

  unsigned get_pixel_height()
  {
    return height;
  }

  unsigned get_pixel_width()
  {
    return width;
  }

  void set_pixel(unsigned x, unsigned y)
  {
    assert(x < width && y < height);
    pixels[x][y] = true; 
  }

  void clear_pixel(unsigned x, unsigned y)
  {
    assert(x < width && y < height);
    pixels[x][y] = false; 
  }

  void clear_pixel_wrap(int x, int y)
  {
    clear_pixel(abs(x % width), abs(y % height));
  }

  void set_pixel_wrap(int x, int y)
  {
    set_pixel(abs(x % width), abs(y % height));
  }

  void render()
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

Screen * initWindow()
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

  return new Screen_impl(WINDOW_PIXEL_WIDTH, WINDOW_PIXEL_HEIGHT);
}
