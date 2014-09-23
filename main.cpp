#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "main.h"
#include "containers.h"

// Globals
int clickedTimes = 0;
int gWidth = 600;
int gHeight = 600;
int timer = 0;
enum Modes {Dot, Line, Poly};
Modes mouseMode = Dot;
std::vector<Drawable> drawable;
std::vector<Drawable>::iterator i;
std::vector<Vertex> currentVertices;
Vertex currentVertex;

void drawDrawable(void)
{
	for (i = drawable.begin(); i != drawable.end(); i++) {
		glBegin(i->type);
		std::vector<Vertex>::iterator j;
		for (j = i->vertices.begin(); j != i->vertices.end(); j++) {
			if (j->position.x < 0 || j->position.x > gWidth) { // boundry check on x
				j->direction.x *= -1;
			}
			if (j->position.y < 0 || j->position.y > gHeight) { // boundry check on y
				j->direction.y *= -1;
			}
			j->position.x += j->direction.x;
			j->position.y += j->direction.y;
			glVertex2f((GLfloat)j->position.x, (GLfloat)j->position.y);
		}
		glEnd();
	}
}

void init(void)
{
	glPointSize(3);
	glLineWidth(3);
	glColor3f(1, 0, 0);
}
void display(void)
{
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = elapsedTime - timer;
	if (deltaTime > 100)
	{
		timer = elapsedTime;
		glClear(GL_COLOR_BUFFER_BIT);
		drawDrawable();
		glFlush();
	}
		glutPostRedisplay();
}

Coord pointSlope(float sx, float sy, float fx, float fy)
{
	float deltaX = fx - sx;
	float deltaY = fy - sy;
	float length = sqrt(deltaX*deltaX + deltaY*deltaY);
	printf("deltaX: %f; deltaY: %f; length %f; xdiff = %f; ydiff = %f\n ",
		deltaX, deltaY, length, deltaX / length, deltaY / length); //debug
	return Coord(deltaX / length, deltaY / length);
}

void mouse(int btn, int state, int x, int y)
{
	switch (btn) {
	case(GLUT_LEFT_BUTTON):
		Coord clickedXY = Coord((float)x, (float)y);
		if (mouseMode == Dot) { // Dot Mode
			if (clickedTimes == 0 && state == GLUT_UP) { // first click
				currentVertex.position = clickedXY;
				glutPostRedisplay();
				clickedTimes++;
			} else if (clickedTimes == 1 && state == GLUT_UP) { // second click
				currentVertex.direction = pointSlope(currentVertex.position.x, currentVertex.position.y, x, y);
				drawable.push_back(currentVertex);
				drawable.back().type = GL_POINTS;
				clickedTimes = 0;
			}
			break;
		}
		else { // Line and Polygon Mode
			if ((clickedTimes % 2 == 0) && state == GLUT_UP) { // first click
				currentVertex.position = clickedXY;
				clickedTimes++;
			}
			else if ((clickedTimes % 2 == 1) && state == GLUT_UP) { // second click
				currentVertex.direction = pointSlope(currentVertex.position.x, currentVertex.position.y, x, y);
				currentVertices.push_back(currentVertex);
				clickedTimes++;
			}
			if (clickedTimes == 4 && state == GLUT_UP && mouseMode == Line) { // final click
				drawable.push_back(currentVertices);
				currentVertices = std::vector<Vertex>(); // empty it
				drawable.back().type = GL_LINES;
				clickedTimes = 0;
			} 
			break;
		}
	}
}

void keyboard(unsigned char btn, int x, int y)
{
	switch (btn) {
	case('q') :
		mouseMode = Dot;
		currentVertices = std::vector<Vertex>(); // empty the in progress vertices
		clickedTimes = 0;
		break;
	case('w') :
		mouseMode = Line;
		currentVertices = std::vector<Vertex>();
		clickedTimes = 0;
		break;
	case('e') :
		mouseMode = Poly;
		currentVertices = std::vector<Vertex>();
		clickedTimes = 0;
		break;
	case(' ') :
		drawable.push_back(currentVertices);
		currentVertices = std::vector<Vertex>(); // empty it
		drawable.back().type = GL_POLYGON;
		clickedTimes = 0;
	}
}

void reshape(int w, int h)
{
	gWidth = w;
	gHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);		  //starts up GLUT
	glutInitWindowSize(600, 600);
	glutCreateWindow("square");	 //creates the window
	gluOrtho2D(0, 600, 600, 0);
	//reshape(600, 600);

	init();
	glutDisplayFunc(display);	   //registers "display" as the display callback function
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();						 //starts the event loop

	return(0);
}




