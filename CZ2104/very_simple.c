/*
 *  very_simple.c
 *  This program draws a white triangle on a black background.
 */


#include <stdlib.h>           
#include <GL/glut.h>              /* glut.h */


void display(void)
{
/* set clear color to black and clear window */

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

/* set drawing/fill color to white.
   three numbers between 0 and 1 represent the intensity 
   of red, green, and blue */

	glColor3f(1.0, 1.0, 1.0);

/* define a triangle or "tri-gon", the window (World coordinates)
   is a quare -1 < x < 1, -1 < y < 1 by default. */

	glBegin(GL_POLYGON);
	 	glVertex2f(0.0, 0.0);
	 	glVertex2f(0.5, 0.0);
	 	glVertex2f(0.0, 0.5);
	glEnd();

/* flush GL buffers */

	glFlush();

}

int main(int argc, char** argv)
{

/* Initialize using the default setting for display mode, 
   window size (raster) and position on the screen. */

	glutInit(&argc,argv);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	glutMainLoop();

}
