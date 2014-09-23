#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "main.h"
#include "containers.h"

// Globals
std::vector<Vertex> vertices;
std::vector<Vertex>::iterator i;
int clickedTimes = 0;
int gWidth = 600;
int gHeight = 600;
int timer = 0;
enum Modes {Dot, Line, Poly};
Modes mouseMode = Dot;
std::vector<Lines> lines;
std::vector<Lines>::iterator l;

void drawPoints(void)
{
	glBegin(GL_POINTS);
	for (i = vertices.begin(); i != vertices.end(); i++) {
		if (i->position.x < 0 || i->position.x > gWidth) { // boundry check on x
			i->direction.x = -1 * (i->direction.x);
		}
		if (i->position.y < 0 || i->position.y > gHeight) { // boundry check on y
			i->direction.y = -1 * (i->direction.y);
		}
		i->position.x += i->direction.x;
		i->position.y += i->direction.y;
		glVertex2f((GLfloat)(*i).position.x, (GLfloat)(*i).position.y);
		//printf("%f %f\n", (*i).position.x, (*i).position.y); //debug
	}
	glEnd();

}

void drawLines(void)
{
	
	for (l = lines.begin(); l != lines.end(); l++) {
		glBegin(GL_LINES);
		std::vector<Coord>::iterator j;
		for (j = l->vertices.begin(); j != l->vertices.end(); j++) {
			if (j->x < 0 || j->x > gWidth) { // boundry check on x
				l->direction.x = -1 * (l->direction.x);
			}
			if (j->y < 0 || j->y > gHeight) { // boundry check on y
				l->direction.y = -1 * (l->direction.y);
			}
			j->x += l->direction.x;
			j->y += l->direction.y;
			glVertex2f((GLfloat)j->x, (GLfloat)j->y);
		}
		glEnd();
	}
	
}

void init(void)
{
	glPointSize(3);
	glLineWidth(3);
}
void display(void)
{
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = elapsedTime - timer;
	if (deltaTime > 100)
	{
		timer = elapsedTime;
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1, 0, 0); // maybe put in init

		drawPoints();
		drawLines();
		glBegin(GL_POINTS); //debug
		glVertex2f(-0.5, -0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
		glEnd();

		glBegin(GL_LINES);	//debug
		glVertex2f(10, 10);
		glVertex2f(50, 50);
		glEnd();


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
		
		if (mouseMode == Dot) {
			Vertex clickPos(Coord((float)x, (float)y));
			if (clickedTimes == 1 && state == GLUT_UP) { // second click
				vertices.back().direction = pointSlope(vertices.back().position.x, vertices.back().position.y, x, y);
				clickedTimes = 0;
			}
			else if (clickedTimes == 0 && state == GLUT_UP) { // first click
				vertices.push_back(clickPos);
				glutPostRedisplay();
				clickedTimes = 1;
			}
			break;
		}
		else if (mouseMode == Line) {
			Coord clickedXY = Coord((float)x, (float)y);
			if (clickedTimes == 0 && state == GLUT_UP) { // first click
				lines.push_back(Lines(clickedXY));
				glutPostRedisplay();
				clickedTimes += 1;
			}
			else if (clickedTimes == 1 && state == GLUT_UP) { // second click
				lines.back().vertices.push_back(clickedXY);
				glutPostRedisplay();
				clickedTimes += 1;
			}
			else if (clickedTimes == 2 && state == GLUT_UP) { // third click
				lines.back().direction = pointSlope(lines.back().vertices[0].x, lines.back().vertices[0].y, x, y);
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
		break;
	case('w') :
		mouseMode = Line;
		break;
	case('e') :
		mouseMode = Poly;
		break;
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




