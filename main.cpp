#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <GL/freeglut.h>
#include <vector>
#include <math.h>
#include "main.h"
#include "containers.h"

// Globals
GLfloat gSize = 3;
const GLfloat gSizeStep = 1;
int clickedTimes = 0;
int gWidth = 600;
int gHeight = 600;
int gTime = 0;
float gSpeed = 0.25;
const float gSpeedStep = 0.001;
float gSpeedPause = 0.01;
bool gPaused = false;
enum Modes {Dot, Line, Poly};
Modes mouseMode = Dot;
std::vector<Drawable> drawable;
std::vector<Drawable>::iterator i;
std::vector<Vertex> currentVertices;
std::vector<Vertex>::iterator j;
Vertex currentVertex;

void updateSizes(void)
{
	glPointSize(gSize);
	glLineWidth(gSize);
}

void drawDrawable(void)
{
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = elapsedTime - gTime;
	gTime = elapsedTime;

	for (i = drawable.begin(); i != drawable.end(); i++) {
		glBegin(i->type);
		for (j = i->vertices.begin(); j != i->vertices.end(); j++) {
			if (j->position.x < 0) j->direction.x = abs(j->direction.x); // boundry check
			if (j->position.x > gWidth) j->direction.x = 0 - abs(j->direction.x); // simply flipping the sign wouldn't work
			if (j->position.y < 0) j->direction.y = abs(j->direction.y);
			if (j->position.y > gHeight) j->direction.y = 0 - abs(j->direction.y);
			
			j->position.x += j->direction.x * deltaTime * gSpeed;
			j->position.y += j->direction.y * deltaTime * gSpeed;
			glColor3f(j->color.x, j->color.y, j->color.z);
			glVertex2f((GLfloat)j->position.x, (GLfloat)j->position.y);
		}
		glEnd();
	}
}


Vector2d pointSlope(float sx, float sy, float fx, float fy)
{
	float deltaX = fx - sx;
	float deltaY = fy - sy;
	float length = sqrt(deltaX*deltaX + deltaY*deltaY);
	//printf("deltaX: %f; deltaY: %f; length %f; xdiff = %f; ydiff = %f\n ",
	//	deltaX, deltaY, length, deltaX / length, deltaY / length); //debug
	return Vector2d(deltaX / length, deltaY / length);
}

Vector3d randomColor(void)
{
	return Vector3d((float)rand() / float(RAND_MAX), (float)rand() / float(RAND_MAX), (float)rand() / float(RAND_MAX));
}

void mouse(int btn, int state, int x, int y)
{
	switch (btn) {
	case(GLUT_LEFT_BUTTON) :
		printf("%d %d \n", x, y);
		Vector2d clickedXY = Vector2d((float)x, (float)y);
		if ((clickedTimes % 2 == 0) && state == GLUT_UP) { // first click
			currentVertex.position = clickedXY;
			clickedTimes++;
		}
		else if ((clickedTimes % 2 == 1) && state == GLUT_UP) { // second click
			currentVertex.direction = pointSlope(currentVertex.position.x, currentVertex.position.y, x, y);
			currentVertex.color = randomColor(); //color
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
	case('1') : // Dot Mode
		mouseMode = Dot;
		currentVertices = std::vector<Vertex>(); // empty the in progress vertices
		clickedTimes = 0;
		break;
	case('2') : // Line Mode
		mouseMode = Line;
		currentVertices = std::vector<Vertex>();
		clickedTimes = 0;
		break;
	case('3') : // Polygon Mode
		mouseMode = Poly;
		currentVertices = std::vector<Vertex>();
		clickedTimes = 0;
		break;
	case('-') : // Speed Down
		gSpeed = (gSpeed - gSpeedStep < 0) ? 0 : gSpeed - gSpeedStep; // no negative speeds
		printf("- pressed\n"); //debug
		break;
	case('+') : // Speed Up
		gSpeed += gSpeedStep;
		printf("+ pressed\n"); //debug
		break;
	case('I') : // DELETE THIS
		gSpeed = (gSpeed - gSpeedStep < 0) ? 0 : gSpeed - 2; // no negative speeds
		break;
	case('O') : // DELETE THIS
		gSpeed = (gSpeed < 0) ? 0 : gSpeed + 2;
		break;
	case('r') : // Reset
		drawable.clear();
		break;
	case('p') : // Pause
		gSpeedPause = gPaused ? gSpeedPause : gSpeed;
		gSpeed = gPaused ? gSpeedPause : 0;
		gPaused = !gPaused;
		break;
	case('t') : // Size Down
		gSize = (gSize - gSizeStep < 1) ? 1 : gSize - gSizeStep;
		updateSizes();
		break;
	case('g') : // Size Up
		gSize += gSizeStep;
		updateSizes();
		break;
	case(' ') : // Finish Polygon
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
	glutTimerFunc(16, timer, 0);
}

void init(void)
{
	updateSizes();
	glColor3f(1, 0, 0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawDrawable();
	//glFlush();
	glutSwapBuffers(); // double buffer
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);		  //starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);

	glutCreateWindow("square");	 //creates the window

	//glutFullScreen();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
	
	init();
	glutDisplayFunc(display);	   //registers "display" as the display callback function
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(16, timer, 0);
	glutMainLoop();						 //starts the event loop

	return(0);
}




