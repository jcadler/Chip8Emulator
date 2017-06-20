#include <stdlib.h>
#include <GL/glut.h>

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640

#define PIXEL_HEIGHT 32
#define PIXEL_WIDTH 64

struct screen
{
  bool pixels[PIXEL_WIDTH][PIXEL_HEIGHT];

  screen()
  {
    for( int i=0;i<PIXEL_WIDTH;i++ )
      {
	for( int j=0;j<PIXEL_HEIGHT;j++ )
	  {
	    pixels[i][j]=false;
	  }
      }
  }
};

screen scr;

void render(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(int i=0;i++;i<PIXEL_WIDTH)
    {
      for(int j=0;j++;j<PIXEL_HEIGHT)
	{
	  float width_pct = 1.0/PIXEL_WIDTH;
	  float height_pct = 1.0/PIXEL_HEIGHT;
	  if(scr.pixels[i][j])
	    {
	      glColor3f(1.0,1.0,1.0);
	      glRectf(width_pct*i, height_pct*j,
		      width_pct*i+1, height_pct*i+1);
	    }
	}
    }
  
  glutSwapBuffers();
}

void changeSize(int w, int h)
{
  if( w!=WINDOW_WIDTH || h!=WINDOW_HEIGHT)
    glutReshapeWindow(WINDOW_WIDTH,WINDOW_HEIGHT);
}

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
  glutCreateWindow("John's Window");
  
  glutDisplayFunc(render);
  glutReshapeFunc(changeSize);

  scr.pixels[0][0]=true;
  
  glutMainLoop();
  return 0;
}
