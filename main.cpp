#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#include "main.h"
#include "containers.h"

// Globals
int clickedTimes = 0;
int gWidth = 600;
int gHeight = 600;
unsigned int gSpeed = 100;
enum Modes {Dot, Line, Poly};
Modes mouseMode = Dot;
std::vector<Drawable> drawable;
std::vector<Drawable>::iterator i;
std::vector<Vertex> currentVertices;
std::vector<Vertex>::iterator j;
Vertex currentVertex;

void drawDrawable(void)
{
	for (i = drawable.begin(); i != drawable.end(); i++) {
		glBegin(i->type);
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
	glClear(GL_COLOR_BUFFER_BIT);
	drawDrawable();
	//glFlush();
	glutSwapBuffers(); // double buffer
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
	case(GLUT_LEFT_BUTTON) :
		printf("%d %d \n", x, y);
		Coord clickedXY = Coord((float)x, (float)y);
		if ((clickedTimes % 2 == 0) && state == GLUT_UP) { // first click
			currentVertex.position = clickedXY;
			clickedTimes++;
		}
		else if ((clickedTimes % 2 == 1) && state == GLUT_UP) { // second click
			currentVertex.direction = pointSlope(currentVertex.position.x, currentVertex.position.y, x, y);
			currentVertices.push_back(currentVertex);
			clickedTimes++;
		}
		if (((clickedTimes == 2 && mouseMode == Dot) || (clickedTimes == 4 && mouseMode == Line)) && state == GLUT_UP) {
			drawable.push_back(currentVertices);
			switch (mouseMode){
			case(Dot):
				drawable.back().type = GL_POINTS;
				break;
			case(Line):
				drawable.back().type = GL_LINES;
			}
			currentVertices = std::vector<Vertex>(); // empty it
			clickedTimes = 0;
		} 
		break;
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
	case('r') : // = // 0x2B is +
		printf("%d\n", gSpeed);
		if (gSpeed > 1) gSpeed-=1;
		break;
	case('f'): // - (on numpad i think)
		printf("df\n", gSpeed);
		gSpeed+=1;
		break;
	case(' ') :
		drawable.push_back(currentVertices);
		drawable.back().type = GL_POLYGON;
		currentVertices = std::vector<Vertex>(); // empty it
		clickedTimes = 0;
		break;
	}
}

void reshape(int w, int h)
{
	gWidth = w;
	gHeight = h;

	//glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//printf("reshaped: %d %d; \n", gWidth, gHeight);
	//glMatrixMode(GL_MODELVIEW);
	//gluOrtho2D(0, w, 0, h);
}


void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(gSpeed, timer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);		  //starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("square");	 //creates the window
	//glutFullScreen();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
	
	init();
	glutDisplayFunc(display);	   //registers "display" as the display callback function
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(gSpeed, timer, 0);
	glutMainLoop();						 //starts the event loop

	return(0);
}




