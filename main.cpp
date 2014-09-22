#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "main.h"

Coord::Coord(float h, float v)
{
	x = h;
	y = v;
}

Coord::Coord()
{
}

Vertex::Vertex(Coord pos)
{
	position = pos;
	direction = Coord(0, 0);
	speed = 0;
}

Vertex::Vertex()
{
}

std::vector<Vertex> vertices;
std::vector<Vertex>::iterator i;

/* display function - GLUT display callback function
*			  clears the screen, draws a square, and displays it
*/

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(3);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (i = vertices.begin(); i != vertices.end(); i++)
	{
		glVertex2f((GLfloat)(*i).position.x, (GLfloat)(*i).position.y);
		printf("%f %f\n", (*i).position.x, (*i).position.y); //debug
	}
	glEnd();
	glBegin(GL_POINTS);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glEnd();
	glFlush();
}

void mouse(int btn, int state, int x, int y)
{
	switch (btn)
	{
	case(GLUT_LEFT_BUTTON) :
		if (state == GLUT_UP) {
			Vertex firstclick(Coord((float)x, (float)y));
			vertices.push_back(firstclick);
			glutPostRedisplay();
		}
	}
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		  //starts up GLUT
	glutInitWindowSize(600, 600);
	glutCreateWindow("square");	 //creates the window
	//glMatrixMode( GL_PROJECTION );
	gluOrtho2D(0, 599, 599, 0);

	glutDisplayFunc(display);	   //registers "display" as the display callback function
	glutMouseFunc(mouse);
	glutMainLoop();						 //starts the event loop

	return(0);									  //return may not be necessary on all compilers
}
